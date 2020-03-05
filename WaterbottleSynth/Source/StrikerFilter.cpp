#include "StrikerFilter.h"

StrikerFilter::StrikerFilter()
{
    for (int i = Striker::Toe; i <= Striker::None; ++i)
    {
        filters.add (new dsp::Convolution);
        loadIRForStriker (static_cast<Striker> (i), filters.getLast());
    }
}

String StrikerFilter::getNameForStriker (Striker s)
{
    if (s == Striker::Toe)
        return "Toe";
    else if (s == Striker::Shoulder)
        return "Shoulder";
    else if (s == Striker::Palm)
        return "Palm";
    else if (s == Striker::OneKnuckle)
        return "One Knuckle";
    else if (s == Striker::Nail)
        return "Nail";
    else if (s == Striker::Knuckles)
        return "Knuckles";
    else if (s == Striker::Knee)
        return "Knee";
    else if (s == Striker::Heel)
        return "Heel";
    else if (s == Striker::Head)
        return "Head";
    else if (s == Striker::GongMallet)
        return "Gong Mallet";
    else if (s == Striker::Elbow)
        return "Elbow";
    else if (s == Striker::DAFx)
        return "DAFx";
    else if (s == Striker::Brush)
        return "Brush";
    else
        return "Ideal";
}

StringArray StrikerFilter::getChoices()
{
    StringArray choices;
    for (int i = Striker::Toe; i <= Striker::None; ++i)
        choices.add (getNameForStriker (static_cast<Striker> (i)));
    
    return choices;
}

void StrikerFilter::loadIRForStriker (Striker s, dsp::Convolution* filter)
{
    if (s == Striker::Toe)
        filter->loadImpulseResponse (BinaryData::Toe_wav, BinaryData::Toe_wavSize, true, true, 0);
    else if (s == Striker::Shoulder)
        filter->loadImpulseResponse (BinaryData::Shoulder_wav, BinaryData::Shoulder_wavSize, true, true, 0);
    else if (s == Striker::Palm)
        filter->loadImpulseResponse (BinaryData::Palm_wav, BinaryData::Palm_wavSize, true, true, 0);
    else if (s == Striker::OneKnuckle)
        filter->loadImpulseResponse (BinaryData::One_Knuckle_wav, BinaryData::One_Knuckle_wavSize, true, true, 0);
    else if (s == Striker::Nail)
        filter->loadImpulseResponse (BinaryData::Nail_wav, BinaryData::Nail_wavSize, true, true, 0);
    else if (s == Striker::Knuckles)
        filter->loadImpulseResponse (BinaryData::Knuckles_wav, BinaryData::Knuckles_wavSize, true, true, 0);
    else if (s == Striker::Knee)
        filter->loadImpulseResponse (BinaryData::Knee_wav, BinaryData::Knee_wavSize, true, true, 0);
    else if (s == Striker::Heel)
        filter->loadImpulseResponse (BinaryData::Heel_wav, BinaryData::Heel_wavSize, true, true, 0);
    else if (s == Striker::Head)
        filter->loadImpulseResponse (BinaryData::Head_wav, BinaryData::Head_wavSize, true, true, 0);
    else if (s == Striker::GongMallet)
        filter->loadImpulseResponse (BinaryData::Gong_Mallet_wav, BinaryData::Gong_Mallet_wavSize, true, true, 0);
    else if (s == Striker::Elbow)
        filter->loadImpulseResponse (BinaryData::Elbow_wav, BinaryData::Elbow_wavSize, true, true, 0);
    else if (s == Striker::DAFx)
        filter->loadImpulseResponse (BinaryData::DAFx_wav, BinaryData::DAFx_wavSize, true, true, 0);
    else if (s == Striker::Brush)
        filter->loadImpulseResponse (BinaryData::Brush_wav, BinaryData::Brush_wavSize, true, true, 0);
    else
        return;
}

void StrikerFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (auto filter : filters)
    {
        dsp::ProcessSpec spec { sampleRate, (uint32) samplesPerBlock, 2 };
        filter->prepare (spec);
    }
}

void StrikerFilter::processBlock (AudioBuffer<float>& buffer)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::ProcessContextReplacing<float> context (block);

    filters[int (currStriker)]->process (context);
}
