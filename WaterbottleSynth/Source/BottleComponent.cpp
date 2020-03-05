#include "BottleComponent.h"

BottleComponent::BottleComponent (WaterbottleSynthAudioProcessor& p) :
    processor (p)
{
    setBufferedToImage (true);

    p.addStickerListener (this);

    bottlePic = Drawable::createFromImageData (BinaryData::Hydroflask_jpg, BinaryData::Hydroflask_jpgSize);
    water = Drawable::createFromImageData (BinaryData::water_png, BinaryData::water_pngSize);
    draw = Drawable::createFromImageData (BinaryData::Drawing_svg, BinaryData::Drawing_svgSize);

    addChildComponent (lasso);
    lasso.setColour (StickerLasso::lassoFillColourID, Colours::black);
    lasso.setColour (StickerLasso::lassoOutlineColourID, Colours::black);
}

BottleComponent::~BottleComponent()
{
    processor.removeStickerListener (this);
}

void BottleComponent::waterAmountChanged (float newAmt)
{
    waterAmt = newAmt;

    MessageManagerLock mml;
    repaint();
}

void BottleComponent::stickersUpdate()
{
    for (auto* sticker : processor.stickers)
        addAndMakeVisible (sticker);

    MessageManagerLock mml;
    repaint();
}

void BottleComponent::mouseDown (const MouseEvent& e)
{
    lasso.beginLasso (e);
}

void BottleComponent::mouseDrag (const MouseEvent& e)
{
    lasso.dragLasso (e);

    MessageManagerLock mml;
    repaint();
}

void BottleComponent::mouseUp (const MouseEvent& e)
{
    if (! lasso.isVisible())
    {
        for (auto* sticker : reverse (processor.stickers))
        {
            if (sticker->getBounds().contains (e.x, e.y))
            {
                processor.stickers.removeObject (sticker);
                break;
            }
        }
    }

    processor.stickers.add (new Sticker (lasso.getBounds(), lasso.getDrawable()));
    auto addedSticker = processor.stickers.getLast();
    if (addedSticker != nullptr)
    {
        addAndMakeVisible (addedSticker);
        const int minDim = 3;

        auto leftOverlap = bottleRect.getX() - addedSticker->getX();
        if (leftOverlap > 0)
        {
            addedSticker->setBounds (bottleRect.getX(), addedSticker->getY(),
                                     jmax (addedSticker->getWidth() - leftOverlap, minDim),
                                     addedSticker->getHeight());
        }

        auto rightOverlap = addedSticker->getRight() - bottleRect.getRight();
        if (rightOverlap > 0)
        {
            addedSticker->setBounds (addedSticker->getX(), addedSticker->getY(),
                                     jmax (addedSticker->getWidth() - rightOverlap, minDim),
                                     addedSticker->getHeight());
        }

        auto topOverlap = bottleRect.getY() - addedSticker->getY();
        if (topOverlap > 0)
        {
            addedSticker->setBounds (bottleRect.getX(), bottleRect.getY(),
                                     addedSticker->getWidth(),
                                     jmax (addedSticker->getHeight() - topOverlap, minDim));
        }

        auto bottomOverlap = addedSticker->getBottom() - bottleRect.getBottom();
        if (bottomOverlap > 0)
        {
            addedSticker->setBounds (addedSticker->getX(), addedSticker->getY(),
                                     addedSticker->getWidth(),
                                     jmax (addedSticker->getHeight() - bottomOverlap, minDim));
        }
    }

    lasso.endLasso();

    MessageManagerLock mml;
    repaint();
}

void BottleComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    bottlePic->drawWithin (g, getLocalBounds().toFloat(), RectanglePlacement::centred, 1.0f);

    // draw water meter
    Rectangle<float> waterRect ((float) getWidth() - 100.0f, 0.0f, 100.0f, (float) getHeight());
    water->drawWithin (g, waterRect, RectanglePlacement::stretchToFit, 1.0f);

    Rectangle<float> coverRect = waterRect.withHeight (waterRect.getHeight() * (1.0f - waterAmt));
    g.setColour (Colours::white);
    g.fillRect (coverRect);

    draw->drawWithin (g, waterRect.reduced (12.0f), RectanglePlacement::stretchToFit, 1.0f);

    // g.setColour (Colours::red);
    // g.drawRect (bottleRect);
}

void BottleComponent::resized()
{
    bottleRect.setBounds ((getWidth() - bottleWidth) / 2, 200, bottleWidth, bottleHeight);
}
