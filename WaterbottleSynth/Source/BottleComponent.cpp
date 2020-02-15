#include "BottleComponent.h"

BottleComponent::BottleComponent()
{
    bottlePic = Drawable::createFromImageData (BinaryData::Hydroflask_jpg, BinaryData::Hydroflask_jpgSize);
    water = Drawable::createFromImageData (BinaryData::water_png, BinaryData::water_pngSize);
    draw = Drawable::createFromImageData (BinaryData::Drawing_svg, BinaryData::Drawing_svgSize);

    addChildComponent (lasso);
    lasso.setColour (StickerLasso::lassoFillColourID, Colours::black);
    lasso.setColour (StickerLasso::lassoOutlineColourID, Colours::black);
}

BottleComponent::~BottleComponent()
{
}

void BottleComponent::waterAmountChanged (float newAmt)
{
    waterAmt = newAmt;
    MessageManager::callAsync ([=] { repaint(); });
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
        for (auto* sticker : stickers)
        {
            if (sticker->getBounds().contains (e.x, e.y))
            {
                stickers.removeObject (sticker);
                break;
            }
        }
    }

    stickers.add (new Sticker (lasso.getBounds()));
    addAndMakeVisible (stickers.getLast());

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
