#include "ModalVoice.h"

ModalVoice::ModalVoice()
{
    for (int ch = 0; ch < 2; ++ch)
    {
        mode[0][ch]  = std::make_unique<BaseMode> ([] (float x) { return -220*x*x*x + 315*x*x - 27.44f*x + 149.8f; }, [] (float x) { return (float) -1.471e6*x*x*x*x + (float) 3.345e6*x*x*x - (float) 2.392e6*x*x + (float) 5.137e5*x + (float) 2.49e4; }, std::complex<float> (1.6375e-4, 8.2776e-4));
        mode[1][ch]  = std::make_unique<BaseMode> ([] (float) { return 735.0f; }, [] (float x) { return (float) -1.471e6*x*x*x*x + (float) 3.345e6*x*x*x - (float) 2.392e6*x*x + (float) 5.137e5*x + (float) 2.49e4; }, std::complex<float> (2.4356e-3, -1.492e-3));
        mode[2][ch]  = std::make_unique<BaseMode> (927.3f,  21474.8f, std::complex<float> (1.4482e-3, 1.9481e-3));
        mode[3][ch]  = std::make_unique<BaseMode> (977.0f,  21694.8f, std::complex<float> (5.4375e-4, 2.6515e-3));
        mode[4][ch]  = std::make_unique<BaseMode> (1371.7f, 14806.6f, std::complex<float> (-9.101e-4, -1.718e-3));
        mode[5][ch]  = std::make_unique<BaseMode> (1739.3f, 13126.2f, std::complex<float> (3.3461e-3, 2.8032e-3));
        mode[6][ch]  = std::make_unique<BaseMode> (1782.3f, 13010.7f, std::complex<float> (9.2404e-4, 1.6469e-3));
        mode[7][ch]  = std::make_unique<BaseMode> (1928.7f, 12464.7f, std::complex<float> (-9.274e-4, 9.0368e-4));
        mode[8][ch]  = std::make_unique<BaseMode> (2151.0f, 11472.7f, std::complex<float> (-1.429e-3, -2.004e-3));
        mode[9][ch]  = std::make_unique<BaseMode> (2182.7f, 11345.0f, std::complex<float> (-6.974e-4, -9.022e-4));
        mode[10][ch] = std::make_unique<BaseMode> (2206.0f, 11250.4f, std::complex<float> (-8.597e-4, -1.253e-3));
        mode[11][ch] = std::make_unique<BaseMode> (2591.3f, 8989.90f, std::complex<float> (3.2111e-4, 1.0081e-3));
        mode[12][ch] = std::make_unique<BaseMode> (2733.3f, 8293.51f, std::complex<float> (-1.467e-3, 1.4958e-3));
        mode[13][ch] = std::make_unique<BaseMode> (3359.3f, 6026.65f, std::complex<float> (1.1023e-3, 1.1865e-3));
        mode[14][ch] = std::make_unique<BaseMode> (3461.7f, 5731.55f, std::complex<float> (-3.712e-4, 3.3302e-3));
        mode[15][ch] = std::make_unique<BaseMode> (3678.0f, 5402.86f, std::complex<float> (-1.659e-3, 1.7872e-4));
        mode[16][ch] = std::make_unique<BaseMode> (3751.0f, 5340.44f, std::complex<float> (-8.134e-4, -2.297e-4));
        mode[17][ch] = std::make_unique<BaseMode> (3977.0f, 5162.93f, std::complex<float> (9.9300e-4, -1.626e-3));
        mode[18][ch] = std::make_unique<BaseMode> (4192.0f, 5026.59f, std::complex<float> (1.2352e-3, 8.9491e-4));
        mode[19][ch] = std::make_unique<BaseMode> (4278.7f, 4957.85f, std::complex<float> (1.0866e-3, 3.1236e-3));
        mode[20][ch] = std::make_unique<BaseMode> (4601.0f, 4804.45f, std::complex<float> (-1.961e-3, -1.558e-3));
        mode[21][ch] = std::make_unique<BaseMode> (4796.0f, 4752.26f, std::complex<float> (2.4649e-3, -2.387e-3));
        mode[22][ch] = std::make_unique<BaseMode> (5216.0f, 4595.37f, std::complex<float> (-1.754e-3, 7.7755e-4));
        mode[23][ch] = std::make_unique<BaseMode> (5319.0f, 4567.85f, std::complex<float> (-3.785e-3, 2.7729e-4));
        mode[24][ch] = std::make_unique<BaseMode> (5653.3f, 4616.46f, std::complex<float> (-1.370e-3, 6.7944e-4));
        mode[25][ch] = std::make_unique<BaseMode> (5962.3f, 4422.07f, std::complex<float> (-2.485e-4, 1.2892e-3));
        mode[26][ch] = std::make_unique<BaseMode> (6056.7f, 4375.13f, std::complex<float> (3.7174e-5, 1.3199e-3));
        mode[27][ch] = std::make_unique<BaseMode> (6955.3f, 3496.75f, std::complex<float> (8.5018e-4, 1.0479e-3));
        mode[28][ch] = std::make_unique<BaseMode> (7344.7f, 3281.70f, std::complex<float> (-3.269e-3, 8.0851e-4));
        mode[29][ch] = std::make_unique<BaseMode> (7429.0f, 3239.40f, std::complex<float> (1.0449e-3, 8.6837e-4));
        mode[30][ch] = std::make_unique<BaseMode> (7599.0f, 3170.67f, std::complex<float> (-1.951e-4, 3.7306e-3));
        mode[31][ch] = std::make_unique<BaseMode> (7761.3f, 3117.73f, std::complex<float> (9.5586e-4, 3.3230e-5));
        mode[32][ch] = std::make_unique<BaseMode> (7945.0f, 3065.84f, std::complex<float> (-1.841e-3, -1.429e-3));
        mode[33][ch] = std::make_unique<BaseMode> (8384.3f, 2912.81f, std::complex<float> (4.1932e-4, 2.4528e-3));
        mode[34][ch] = std::make_unique<BaseMode> (8522.7f, 2880.41f, std::complex<float> (2.4407e-3, -2.483e-3));
        mode[35][ch] = std::make_unique<BaseMode> (8772.0f, 2808.99f, std::complex<float> (-1.047e-3, 5.7032e-4));
        mode[36][ch] = std::make_unique<BaseMode> (9128.3f, 2726.79f, std::complex<float> (-1.937e-3, 1.3581e-3));
        mode[37][ch] = std::make_unique<BaseMode> (9833.7f, 2531.38f, std::complex<float> (1.7622e-4, 1.6890e-3));
        mode[38][ch] = std::make_unique<BaseMode> (10376.f, 2357.52f, std::complex<float> (1.9720e-3, 1.2535e-4));
        mode[39][ch] = std::make_unique<BaseMode> (11209.f, 2091.17f, std::complex<float> (2.3114e-3, 2.8051e-5));
    }
}

bool ModalVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<ModalSound*> (sound) != nullptr;
}

void ModalVoice::setCurrentPlaybackSampleRate (double sampleRate)
{
    SynthesiserVoice::setCurrentPlaybackSampleRate (sampleRate);

    for (int ch = 0; ch < 2; ++ch)
    {
        for (int m = 0; m < numModes; ++m)
            mode[m][ch]->prepare (sampleRate);
    }
}

void ModalVoice::setParameters (float water, float stickers)
{
    waterLevel = water;
    stickersAmt = stickers;

    for (int ch = 0; ch < 2; ++ch)
    {
        for (int m = 0; m < numModes; ++m)
            mode[m][ch]->setParameters (waterLevel, stickersAmt);
    }
}

void ModalVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    auto freq = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    auto freqMult = freq / mode[0][0]->getBaseFreq();

    for (int ch = 0; ch < 2; ++ch)
    {
        for (int m = 0; m < numModes; ++m)
            mode[m][ch]->triggerNote (freqMult, velocity);
    }
}

void ModalVoice::stopNote (float /*velocity*/, bool  allowTailOff)
{
    clearCurrentNote();
}

void ModalVoice::renderNextBlock (AudioSampleBuffer& buffer, int startSample, int numSamples)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            for (int m = 0; m < 40; ++m)
                x[n] += mode[m][ch]->getNextSample();
        }
    }

    // buffer.applyGain (0.5f);
}
