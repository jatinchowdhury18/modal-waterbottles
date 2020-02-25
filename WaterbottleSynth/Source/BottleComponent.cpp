#include "BottleComponent.h"

BottleComponent::BottleComponent (WaterbottleSynthAudioProcessor& p) :
    processor (p)
{
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
    MessageManager::callAsync ([=] { repaint(); });
}

void BottleComponent::stickersUpdate()
{
    for (auto* sticker : processor.stickers)
        addAndMakeVisible (sticker);

    repaint();
}

void BottleComponent::mouseDown (const MouseEvent& e)
{
    lasso.beginLasso (e);
}

void BottleComponent::mouseDrag (const MouseEvent& e)
{
    lasso.dragLasso (e);
    repaint();
}

void BottleComponent::mouseUp (const MouseEvent& e)
{
    if (! lasso.isVisible())
    {
        for (auto* sticker : processor.stickers)
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

        Rectangle<int> bottleRect (190, 130, 120, 255);
        auto leftOverlap = bottleRect.getX() - addedSticker->getX();
        if (leftOverlap > 0)
        {
            addedSticker->setBounds (bottleRect.getX(), addedSticker->getY(),
                                     addedSticker->getWidth() - leftOverlap,
                                     addedSticker->getHeight());
        }

        auto rightOverlap = addedSticker->getRight() - bottleRect.getRight();
        if (rightOverlap > 0)
        {
            addedSticker->setBounds (addedSticker->getX(), addedSticker->getY(),
                                     addedSticker->getWidth() - rightOverlap,
                                     addedSticker->getHeight());
        }

        auto topOverlap = bottleRect.getY() - addedSticker->getY();
        if (topOverlap > 0)
        {
            addedSticker->setBounds (bottleRect.getX(), bottleRect.getY(),
                                     addedSticker->getWidth(),
                                     addedSticker->getHeight() - topOverlap);
        }

        auto bottomOverlap = addedSticker->getBottom() - bottleRect.getBottom();
        if (bottomOverlap > 0)
        {
            addedSticker->setBounds (addedSticker->getX(), addedSticker->getY(),
                                     addedSticker->getWidth(),
                                     addedSticker->getHeight() - bottomOverlap);
        }
    }

    lasso.endLasso();

    repaint();
}

void BottleComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    bottlePic->drawWithin (g, getLocalBounds().toFloat(), RectanglePlacement::centred, 1.0f);

    // draw water meter
    Rectangle<float> waterRect (400.0f, 0.0f, 100.0f, (float) getHeight());
    water->drawWithin (g, waterRect, RectanglePlacement::stretchToFit, 1.0f);

    Rectangle<float> coverRect = waterRect.withHeight (waterRect.getHeight() * (1.0f - waterAmt));
    g.setColour (Colours::white);
    g.fillRect (coverRect);

    draw->drawWithin (g, waterRect.reduced (12.0f), RectanglePlacement::stretchToFit, 1.0f);
}

void BottleComponent::resized()
{
}
