#ifndef STICKERS_H_INCLUDED
#define STICKERS_H_INCLUDED

#include "JuceHeader.h"

class Sticker : public Component
{
public:
    Sticker (Rectangle<int> bounds, const Drawable* pic = nullptr)
    {
        if (pic == nullptr)
            dafx = Drawable::createFromImageData (BinaryData::dafx_sticker_jpg, BinaryData::dafx_sticker_jpgSize);
        else
            dafx = pic->createCopy();

        setBounds (bounds);
        setInterceptsMouseClicks (false, false);
    }

    ~Sticker() {}

    void paint (Graphics& g)
    {
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

    const Drawable* getDrawable() const
    {
        return dafx.get();
    }

    void beginLasso (const MouseEvent& e)
    {
        if (choice % 3 == 0)
            dafx = Drawable::createFromImageData (BinaryData::dafx_sticker_jpg, BinaryData::dafx_sticker_jpgSize);
        if (choice % 3 == 1)
            dafx = Drawable::createFromImageData (BinaryData::ccrma_logo_png, BinaryData::ccrma_logo_pngSize);
        if (choice % 3 == 2)
            dafx = Drawable::createFromImageData (BinaryData::matlab_jpg, BinaryData::matlab_jpgSize);

        choice++;

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

    int choice = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StickerLasso)
};

#endif // STICKERS_H_INCLUDED
