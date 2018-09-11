#include "World.hpp"
#include "SpriteNode.hpp"
#include <memory>

World::World(sf::RenderTarget& target, TextureHolder& textures, FontHolder& fonts)
    : mTarget(target),
      mTextures(textures),
      mFonts(fonts)
{
    buildWorld();
}

void World::update(sf::Time dt)
{
    while(!mCommandQueue.isEmpty())
        mSceneGraph.executeCommand(mCommandQueue.pop(), dt);
}

void World::draw()
{
    mTarget.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::buildWorld()
{
    for(int i=0; i < LayerCount; ++i)
    {
        Category::Type category = (i == LowerAir) ? Category::AirLayer : Category::None; // LowerAir for particles, UpperAir for collideables
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    sf::Texture& background = mTextures.get(Textures::Background);
    background.setRepeated(true);
    sf::IntRect backgroundRect(0, 0, 1024, 768);

    std::unique_ptr<SpriteNode> galaxyBackground(new SpriteNode(background, backgroundRect));
    galaxyBackground->setPosition(0.f, 0.f);
    mSceneLayers[Background]->attachChild(std::move(galaxyBackground));

    std::unique_ptr<Aircraft> playerAircraft(new Aircraft(Aircraft::Ally, mTextures, mFonts));
    playerAircraft->setPosition(512.f, 600.f); // Change to viewCenter
    playerAircraft->setIdentifier(0);
    mSceneLayers[UpperAir]->attachChild(std::move(playerAircraft));
}
