#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "Category.hpp"
#include "Command.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <vector>
class CommandQueue;


class SceneNode : public sf::Drawable, public sf::Transformable
{
    public:
        using Ptr = std::unique_ptr<SceneNode>;

        explicit                SceneNode(Category::Type = Category::None);
        virtual                 ~SceneNode() = default;
        sf::Vector2f            getWorldPosition() const;
        sf::Transform           getWorldTransform() const;
        sf::Transform           getWorldInverseTransform() const;
        void                    attachChild(Ptr child);
        void                    eraseChild(Ptr child);
        void                    update(sf::Time, CommandQueue&);
        void                    executeCommand(const Command&, sf::Time);
        Category::Type          getCategories() const;
        virtual sf::FloatRect   getBoundingRect() const;
        void                    removeWrecks();
        virtual bool            isMarkedForRemoval() const;

    protected:
        void            addCategories(Category::Type);

    private:
        virtual void    draw(sf::RenderTarget&, sf::RenderStates) const override final;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
        void            drawChildren(sf::RenderTarget&, sf::RenderStates) const;
        virtual void    updateCurrent(sf::Time, CommandQueue&);
        void            updateChildren(sf::Time, CommandQueue&);
        virtual void    onRemoval();

        Category::Type      mCategory;
        SceneNode*          mParent;
        std::vector<Ptr>    mChildren;
};

#endif // SCENENODE_HPP
