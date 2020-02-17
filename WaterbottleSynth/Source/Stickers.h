#ifndef STICKERS_H_INCLUDED
#define STICKERS_H_INCLUDED

#include "JuceHeader.h"

class Sticker : public Component
{
public:
    Sticker (Rectangle<int> bounds)
    {
        dafx = Drawable::createFromImageData (BinaryData::dafx_sticker_jpg, BinaryData::dafx_sticker_jpgSize);

        setBounds (bounds);
        setInterceptsMouseClicks (false, false);
    }

    ~Sticker() {}

    void paint (Graphics& g)
    {
        g.fillAll (Colours::black);
        dafx->drawWithin (g, getLocalBounds().toFloat(), RectanglePlacement::stretchToFit, 1.0f);
    }

private:
    std::unique_ptr<Drawable> dafx;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sticker)
};

class StickerLasso : public Component
{
public:
    StickerLasso()
    {
        dafx = Drawable::createFromImageData (BinaryData::dafx_sticker_jpg, BinaryData::dafx_sticker_jpgSize);
    }

    ~StickerLasso() {}

    void beginLasso (const MouseEvent& e)
    {
        setSize (0, 0);
        dragStartPos = e.getMouseDownPosition();
        toFront (false);
    }

    void dragLasso (const MouseEvent& e)
    {
        setBounds (Rectangle<int> (dragStartPos, e.getPosition()));
        setVisible (true);
    }

    void endLasso()
    {
        setVisible (false);
    }

    void paint (Graphics& g)
    {
        // getLookAndFeel().drawLasso (g, *this);
        g.fillAll (Colours::black);
        dafx->drawWithin (g, getLocalBounds().toFloat(), RectanglePlacement::stretchToFit, 1.0f);
    }

    enum ColourIDs
    {
        lassoFillColourID    = 0x1000440,
        lassoOutlineColourID = 0x1000441,
    };

private:
    Point<int> dragStartPos;
    std::unique_ptr<Drawable> dafx;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StickerLasso)
};

#endif // STICKERS_H_INCLUDED
