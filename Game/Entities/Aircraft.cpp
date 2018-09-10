#include "Aircraft.hpp"

Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts)
    : Entity(100), //Change this later
      mType(type),
      mFireRateLevel(1),
      mSpreadLevel(1),
      mMissileAmmo(2),
      mIdentifier(0)
{
}

void Aircraft::updateCurrent(sf::Time dt)
{
    // Add more
    Entity::updateCurrent(dt);
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

Category::Type Aircraft::getCategory()
{
    if(mType == Ally)
        return Category::PlayerAircraft;
    else
        return Category::EnemyAircraft;
}

void Aircraft::increaseFireRate()
{
    mFireRateLevel += 1;
}

void Aircraft::increaseSpread()
{
    mSpreadLevel += 1;
}

int Aircraft::getMissileAmmo()
{
    return mMissileAmmo;
}

void Aircraft::setMissileAmmo(int missiles)
{
    mMissileAmmo = missiles;
}

int  Aircraft::getIdentifier()
{
    return mIdentifier;
}

void Aircraft::setIdentifier(int id)
{
    mIdentifier = id;
}

float Aircraft::getMaxSpeed() const
{
    //
}
