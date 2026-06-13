#include <SDL/SDL.hpp>
#include <SDL.h>
#include <game.hpp>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib> // rand()
#include <math/vector.hpp>
#include <cmath> // fmod
#define ACCELLARATION 2.5
#define ACCELLARATION_MAX 10.0
#define PROJECTILE_COOLDOWN 1
#define ROTATION_SPEED 300.0
namespace Asteroids
{
    // Entity
    const Vector2 Entity::position() const
    {
        return m_position;
    }
    const Vector2 Entity::velocity() const
    {
        return m_velocity;
    }
    const double Entity::rotation() const
    {
        return m_rotation;
    }
    const SDL::Image *Entity::sprite() const
    {
        return m_sprite;
    }
    void Entity::set_position(Vector2 new_pos)
    {
        m_position.x = fmod(fmod(new_pos.x, static_cast<double>(SCREEN_WIDTH)) + SCREEN_WIDTH, SCREEN_WIDTH);
        m_position.y = fmod(fmod(new_pos.y, static_cast<double>(SCREEN_HEIGHT)) + SCREEN_HEIGHT, SCREEN_HEIGHT);
    }
    void Entity::update(double delta_seconds)
    {
        set_position(position() + velocity() * delta_seconds);
    }
    void Entity::rotate(double degrees)
    {
        m_rotation += degrees;
    }
    void Entity::load_sprite(SDL::Image *sprite)
    {
        m_sprite = sprite;
    }

    const std::string Entity::sprite_name()
    {
        return m_sprite_name;
    }
    // End Entity
    // IDamagable
    const double IDamageable::radius() const
    {
        return m_circle_radius;
    }
    void IDamageable::receive_damage(int damage){
        m_health -= damage;
        
    }
    // End IDamagable
    // Player
    void Player::update(double delta_seconds)
    {
        Entity::update(delta_seconds);
        m_cooldown_timer -= delta_seconds;
        m_cooldown_timer = std::max(0.0, m_cooldown_timer);
        if (m_is_accelarating)
        {
            load_sprite(Game::get_instance()->get_sprite("ship_accel"));
        }
        else
        {
            load_sprite(Game::get_instance()->get_sprite("ship_idle"));
            m_accelaration -= 0.2;
            m_accelaration = m_accelaration <= 0.0 ? 0.0 : m_accelaration;
        }
        m_is_accelarating = false;
    }

    Vector2 Player::face_direction() const{
        auto direction = Vector2{0.0, -1.0};
        return direction.rotate(m_rotation);
    }

    void Player::accellarate()
    {
        m_is_accelarating = true;
        m_accelaration += ACCELLARATION;
        m_accelaration = std::clamp(m_accelaration, 0.0, ACCELLARATION_MAX);
        m_velocity = m_velocity + (face_direction() * m_accelaration);
    }
    void Player::shoot_laser(){
        m_cooldown_timer = PROJECTILE_COOLDOWN;
    }

    bool Player::is_on_cooldown(){
        return m_cooldown_timer > 0.0;
    }

    void Player::on_death()
    {
    }
    // End Player
    // ASteroid
    Asteroid::Asteroid(const unsigned char asteroid_size, Vector2 position, Vector2 velocity, std::string sprite_name = "", SDL::Image *sprite) : Entity{position, velocity, 0.0, sprite_name, sprite},
                                                                                                                                                  IDamageable{(double)asteroid_size * 10.0},
                                                                                                                                                  m_asteroid_size{asteroid_size}
    {
        switch (asteroid_size)
        {
        case 1:
            m_sprite_name = "asteroid_s";
            break;
        case 2:
            m_sprite_name = "asteroid_m";
            break;
        default:
            m_sprite_name = "asteroid_s";
            break;
        }
    }

    void Asteroid::on_death()
    {
        if (m_asteroid_size > 1)
        {
            auto rnd = std::rand() % 3 + 2;
            unsigned char new_size = m_asteroid_size -1;
            for (size_t i = 0; i < rnd; i++)
            {
                auto random_rotation = static_cast<double>(std::rand() % 360);
                auto ast = std::make_unique<Asteroid>(Asteroid{new_size,m_position, m_velocity.rotate(random_rotation)});
                Game::get_instance()->spawn_asteroid(std::move(ast));
            }
            
            // TODO implement random number and directions
            
        }
       
    }
    // End Asteroid
    //Projectile
    Projectile::Projectile(Vector2 position, Vector2 velocity, double rotation): 
    Entity{position,velocity,rotation, "laser"},
    IDamageable{5.0}{}

    void Projectile::on_death() {
        // Game::get_instance()->despawn_projectile(this);
    }
    //End Projectile
    // Game
    void Game::setup()
    {
        // Set Instance
        m_instance = this;
        // Loading Textures
        m_textures = std::map<std::string, std::unique_ptr<SDL::Image>>{};
        m_textures.emplace("ship_idle", std::make_unique<SDL::Image>(load_image("assets/ship01.png")));
        m_textures.emplace("ship_accel", std::make_unique<SDL::Image>(load_image("assets/ship02.png")));
        m_textures.emplace("asteroid_s", std::make_unique<SDL::Image>(load_image("assets/asteroid02.png")));
        m_textures.emplace("asteroid_m", std::make_unique<SDL::Image>(load_image("assets/asteroid01.png")));
        m_textures.emplace("laser", std::make_unique<SDL::Image>(load_image("assets/laser.png")));
        // Initialize Entities
        m_asteroids = std::vector<std::unique_ptr<Asteroid>>{};
        // Initialize Player
        auto i = m_textures.find("ship_idle");

        auto player_entity = Player{(i->second.get())};
        player = std::make_unique<Player>(player_entity);

        // Spawn Asteroids
        spawn_asteroid(std::make_unique<Asteroid>(Asteroid{
            1,
            Vector2{50.0, 2.0},
            Vector2{1.0, 7.2},
        }));
        spawn_asteroid(std::make_unique<Asteroid>(Asteroid{
            2,
            Vector2{50.0, 2.0},
            Vector2{10.0, -7.2},
        }));


    }

    Game *Game::get_instance()
    {
        return Game::m_instance;
    }

    SDL::Image *Game::get_sprite(std::string name) const
    {
        auto it = m_textures.find(name);
        if (it == m_textures.end())
        {
            std::cerr << "texture not found: " << name << '\n';
            return nullptr;
        }
        return it->second.get();
    }
    void Game::render(SDL::Renderer &renderer) const
    {
        renderer.render(*player);
        for (auto &entity : m_asteroids)
        {
            renderer.render(*entity);
        }
        for (auto &projectile : m_projectiles)
        {
            renderer.render(*projectile);
        }
    }

    void Game::spawn_asteroid(std::unique_ptr<Asteroid> entity)
    {
        entity.get()->load_sprite(get_sprite(entity.get()->sprite_name()));
        m_asteroids.push_back(std::move(entity));
    }

    void Game::despawn_asteroid(const Asteroid* asteroid)
    {
        auto condition = [&asteroid](const std::unique_ptr<Asteroid> &entity) {
            return entity.get() == asteroid;
        };
        m_asteroids.erase(std::remove_if(m_asteroids.begin(), m_asteroids.end(), condition), m_asteroids.end());
    }

    void Game::despawn_projectile(const Projectile* projectile)
    {
        auto condition = [&projectile](const std::unique_ptr<Projectile> &ptr) {
            return ptr.get() == projectile;
        };
        m_projectiles.erase(std::remove_if(m_projectiles.begin(), m_projectiles.end(), condition), m_projectiles.end());
    }

    void Game::check_collision()
    {
        
        for (const auto &asteroid : m_asteroids)
        {
            if (auto dmgable = dynamic_cast<IDamageable *>(asteroid.get()))
            {
                auto distance = dmgable->radius() + player.get()->radius();
                if (distance >= (player.get()->position() - asteroid.get()->position()).magnitude())
                {
                    std::cout << "Player was hit!!!";
                    quit();
                }
                for (const auto& projectile : m_projectiles)
                {
                    auto distance = dmgable->radius() + projectile.get()->radius();
                    if (distance >= (projectile.get()->position() - asteroid.get()->position()).magnitude())
                    {
                        std::cout << "Laser hit Asteroid!!!";
                        dmgable->receive_damage(1);
                        projectile->receive_damage(1);
                    }
                }
            }
        }

        std::vector<std::unique_ptr<Projectile>> removed_projectiles;
        for (auto it = m_projectiles.begin(); it != m_projectiles.end(); ) {
            if ((*it)->is_dead()) {
                removed_projectiles.push_back(std::move(*it));
                it = m_projectiles.erase(it);
            } else {
                ++it;
            }
        }
        std::vector<std::unique_ptr<Asteroid>> removed_asteroids;
        for (auto it = m_asteroids.begin(); it != m_asteroids.end(); ) {
            if ((*it)->is_dead()) {
                removed_asteroids.push_back(std::move(*it));
                it = m_asteroids.erase(it);
            } else {
                ++it;
            }
        }
        for (auto &proj : removed_projectiles) {
            proj->on_death();
        }
        for (auto &ast : removed_asteroids) {
            ast->on_death();
        }
       
    }

    void Game::update(double delta_seconds)
    {
        // TODO: Implement game logic update
        check_collision();

        if (is_key_down(SDLK_a))
        {
            player->rotate(-ROTATION_SPEED * delta_seconds);
        }
        if (is_key_down(SDLK_d))
        {
            player->rotate(ROTATION_SPEED * delta_seconds);
        }
        if (is_key_down(SDLK_w))
        {
            player->accellarate();
            // player->load_sprite(get_sprite("ship_accel"));
        }
        if(is_key_down(SDLK_SPACE)&& !(player.get()->is_on_cooldown())){
            auto ptr = std::make_unique<Projectile>(
                Projectile{player.get()->position()+ player.get()->face_direction()*10.0,
                    player.get()->face_direction() * 300.0, 
                    player.get()->rotation()});
            ptr.get()->load_sprite(get_sprite(ptr.get()->sprite_name()));
            m_projectiles.push_back(std::move(ptr));
            player->shoot_laser();
        }
        

        // Update All Entites
        for (auto &asteroid : m_asteroids)
        {
            asteroid->update(delta_seconds);
        }
        for (auto &projectile : m_projectiles)
        {
            projectile->update(delta_seconds);
        }
        player->update(delta_seconds);
    }
    // End Game
} // End Asteroids
