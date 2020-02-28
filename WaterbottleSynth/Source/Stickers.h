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
        if (choice % numStickers == 0)
            dafx = Drawable::createFromImageData (BinaryData::dafx_sticker_jpg, BinaryData::dafx_sticker_jpgSize);
        if (choice % numStickers == 1)
            dafx = Drawable::createFromImageData (BinaryData::ccrma_logo_png, BinaryData::ccrma_logo_pngSize);
        if (choice % numStickers == 2)
            dafx = Drawable::createFromImageData (BinaryData::matlab_jpg, BinaryData::matlab_jpgSize);
        if (choice % numStickers == 3)
            dafx = Drawable::createFromImageData (BinaryData::polytec_jpg, BinaryData::polytec_jpgSize);
        if (choice % numStickers == 4)
            dafx = Drawable::createFromImageData (BinaryData::python_jpg, BinaryData::python_jpgSize);
        if (choice % numStickers == 5)
            dafx = Drawable::createFromImageData (BinaryData::NILogo_jpg, BinaryData::NILogo_jpgSize);
        if (choice % numStickers == 6)
            dafx = Drawable::createFromImageData (BinaryData::JUCE_png, BinaryData::JUCE_pngSize);

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
    const int numStickers = 7;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StickerLasso)
};

#endif // STICKERS_H_INCLUDED
