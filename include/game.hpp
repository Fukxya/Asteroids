#pragma once
#include <SDL/SDL.hpp> // Application,  Renderer
#include <math/vector.hpp> //Vector2 
#include <string>
#include <vector>
#include <memory>
#include <map>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

namespace Asteroids{
   
    class Entity {
        protected: 
            Vector2 m_position;
            Vector2 m_velocity;
            double m_rotation;
            std::string m_sprite_name;
            SDL::Image* m_sprite;
           
        public:
            Entity(Vector2 position, Vector2 velocity, double rotation,std::string sprite_name, SDL::Image* sprite = nullptr):
            m_position{position},
            m_velocity{velocity},
            m_rotation{rotation},
            m_sprite_name{sprite_name},
            m_sprite{sprite}
            {}
            virtual ~Entity() = default;

            void set_position(Vector2 new_pos);
            virtual void update(double delta_seconds);
            void rotate(double degrees);
            const Vector2 position() const;
            const Vector2 velocity() const;
            const double rotation() const;
            const std::string sprite_name();
            void load_sprite(SDL::Image* sprite);
            const SDL::Image* sprite() const;

    };
    class IDamageable {
        private:
            double m_circle_radius;
            int m_health;
        public:
            IDamageable(const double radius, const int health = 1): m_circle_radius{radius}, m_health{health}{}    
            void receive_damage(const int damage);
            virtual void on_death() = 0;
            const double radius() const;
            // virtual ~IDamageable();
    };

    class Player: public Entity, public IDamageable {
        private:
            double m_accelaration;
            bool m_is_accelarating;
            double m_cooldown_timer;
        public:
            Player(SDL::Image* sprite): 
            Entity{Vector2{SCREEN_WIDTH/2.0, SCREEN_WIDTH/2.0},
            Vector2{0.0, 0.0}, 0.0, "ship_idle",sprite},
            m_accelaration{0.0},
            m_is_accelarating{false},
            IDamageable{
                20.0
            }
            {}
            void accellarate();
            void shoot_laser();
            bool is_on_cooldown();
            void update(double delta_seconds) override;
            Vector2 face_direction() const;
            void on_death() override;
            // void update(double delta_seconds) override ;

    };
    
    
    class Asteroid : public Entity,  public IDamageable{
        private:
            const unsigned char m_asteroid_size;
        public:
            Asteroid(const unsigned char asteroid_size, Vector2 position, Vector2 velocity,std::string sprite_name, SDL::Image* sprite = nullptr);
            void on_death() override;
    };

    class Projectile: public Entity, public IDamageable{
        public:
            Projectile(Vector2 position, Vector2 velocity, double rotation);
            void on_death() override;
    };

    class Game : public SDL::Application{
        private:
            std::map<std::string,std::unique_ptr<SDL::Image>> m_textures;
            std::vector<std::unique_ptr<Entity>> m_asteroids;
            std::vector<std::unique_ptr<Projectile>> m_projectiles;
            std::unique_ptr<Player> player;
            inline static Game* m_instance;
        public:

            Game(const SDL::Window& window, SDL::Renderer& renderer)
                : SDL::Application{window, renderer} {
                    setup();
                }
            
            void update(double delta_seconds) override;
            void render(SDL::Renderer& renderer) const override;
            void setup();
            SDL::Image* get_sprite(std::string name) const;
            void spawn_asteroid(std::unique_ptr<Entity> entity);
            void despawn_asteroid(const Asteroid* asteroid);
            void despawn_projectile(const Projectile *projectile);
            static Game *get_instance();
            void check_collision();
    };

}//End Asteroirds