#include "GUI_InputBox.hpp"
#include "../Utility.hpp"
#include <SFML/Graphics/Color.hpp>

GUI_InputBox::GUI_InputBox(std::string& output, sf::Vector2f boxSize, int maxCharacters, bool isForced, const FontHolder& fonts)
    : mOutput(output),
      mBox(boxSize),
      mInputCursor(sf::Vector2f(1.f, boxSize.y - 4.f)),
      mMaxCharacters(maxCharacters),
      mInputPosition(0),
      mIsForced(isForced),
      mShowCursor(false)
{
    if(mIsForced)
        activate();

    mBox.setFillColor(sf::Color(61, 189, 204, 180));

    mInputCursor.setFillColor(sf::Color::Black);
    mInputCursor.setPosition(2.f, 2.f);

    mText.setFont(fonts.get(Fonts::Sansation));
    mText.setCharacterSize(static_cast<unsigned int>(boxSize.y - 10.f));
    mText.setPosition(2.f, 2.f);
}

bool GUI_InputBox::isSelectable() const
{
    return false;
}

void GUI_InputBox::activate()
{
    GUIObject::activate();
    mShowCursor = true;
}

void GUI_InputBox::deactivate()
{
    GUIObject::deactivate();
    mShowCursor = false;
}

void GUI_InputBox::handleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyReleased:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Left:
                    if(mInputPosition > 0)
                    {
                        --mInputPosition;
                        computeCursorPosition();
                    }
                    return;

                case sf::Keyboard::Right:
                    if(mInputPosition < mString.size())
                    {
                        ++mInputPosition;
                        computeCursorPosition();
                    }
                    return;

                case sf::Keyboard::BackSpace:
                    if(mInputPosition > 0)
                    {
                        mString.erase(--mInputPosition, 1);
                        mText.setString(mString);
                        computeCursorPosition();
                    }
                    return;

                case sf::Keyboard::Enter:
                    if(!mIsForced || mString.size() >= 1) // If isForced then user has to enter at least 1 character
                    {
                        mOutput.assign(mString);
                        deactivate();
                    }
                    return;
            }
            break;
        }

        case sf::Event::MouseButtonReleased:
        {
            if(!mIsForced || mString.size() >= 1) // If isForced then user has to enter at least 1 character
            {
                sf::Vector2i tempPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f mousePosition = static_cast<sf::Vector2f>(tempPos);

                if(!getBoundingRect().contains(mousePosition))
                    deactivate();
            }
            break;
        }

        case sf::Event::TextEntered:
        {
            if(event.text.unicode < 128 && isPrintable(event.text.unicode) && mString.size() < mMaxCharacters) // Don't insert backspace char
            {
                mString.insert(mInputPosition++, 1, static_cast<char>(event.text.unicode));
                mText.setString(mString);
                computeCursorPosition();
            }
            break;
        }
    }
}

sf::FloatRect GUI_InputBox::getBoundingRect() const
{
    return getTransform().transformRect(mBox.getGlobalBounds());
}

void GUI_InputBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mBox, states);
    target.draw(mText, states);
    if(isActive() && mShowCursor)
        target.draw(mInputCursor, states);
}

void GUI_InputBox::update(sf::Time dt)
{
    // Make cursor blink
    if(isActive())
    {
        mAccumulatedTime += dt;
        if(mAccumulatedTime >= sf::seconds(0.4f))
        {
            mAccumulatedTime = sf::Time::Zero;
            mShowCursor = !mShowCursor;
        }
    }
}

sf::FloatRect GUI_InputBox::getLocalBounds() const
{
    return mBox.getLocalBounds();
}

void GUI_InputBox::computeCursorPosition()
{
    if(mInputPosition == 0)
        mInputCursor.setPosition(2.f, 2.f);
    else
    {
        sf::Vector2f newPosition = mText.findCharacterPos(mInputPosition);
        mInputCursor.setPosition(newPosition);
    }
}

bool GUI_InputBox::isPrintable(sf::Uint32 unicode) const
{
    if(unicode >= 0 && unicode < 32)
        return false;

    return true;
}
