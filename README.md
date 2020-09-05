# Modal Waterbottles

[![Build Status](https://travis-ci.com/jatinchowdhury18/modal-waterbottles.svg?token=Ub9niJrqG1Br1qaaxp7E&branch=master)](https://travis-ci.com/jatinchowdhury18/modal-waterbottles)

This repository contains materials for performing modal synthesis of 
waterbottles. Currently we have measurements for two waterbottles,
a 32 oz. Wide Mouth [HydroFlask](https://www.hydroflask.com/32-oz-wide-mouth/color,cobalt,a,92,o,53), and a DAFx 2019 bottle.
We measure with differing amounts of water and stickers on the bottles.

The organization of the repository is as follows:

    ./                       : Python scripts for analyzing waterbottle recordings
    Audio/                   : Audio examples (measured and synthesized)
    BespokeWaterbottleSynth/ : A bespoke waterbottle synthesizer plugin
    Bin/                     : Builds of the both synthesizer plugins
    Figures/                 : Plots of waterbottle modal analysis
    Impact_Wavs/             : Recordings of waterbottle impact drivers
    Paper/                   : DAFx paper on waterbottle synthesis
    WaterbottleSynth/        : A HydroFlask waterbottle synthesizer plugin
    Waterbottles-web/        : A web app for analyzing waterbottle recordings

This repository also comprises supplementary material for the DAFx 2020
paper "Water Bottle Synthesis with Modal Signal Processing", by Jatin
Chowdhury, Elliot Canfield-Dafilou, and Mark Rau.

Associated materials:
- [DAFx Paper](https://dafx2020.mdw.ac.at/proceedings/papers/DAFx2020_paper_24.pdf)
- [Audio examples webpage](https://ccrma.stanford.edu/~jatin/Waterbottles/)
- [Waterbottle analysis web app](http://ccrmawaterbottles.pythonanywhere.com/)
- [Video demo](https://youtu.be/MwhBluJqePE)

## Plugins

The plugins in this repository are built using the
[JUCE](https://github.com/WeAreROLI/JUCE) framework, specifically JUCE
v5.4.4. Both plugins can be played through their on-screen keyboards,
as well as through a connected MIDI keyboard. The plugins are currently
available in the following formats: VST, VST3, AU, Standalone. To download
the plugins see the [`Bin/` folder](./Bin/).

### WaterbottleSynth

<img src="https://raw.githubusercontent.com/jatinchowdhury18/modal-waterbottles/master/Figures/WaterbottleSynthPlugin.PNG" alt="Pic" width="500">

The WaterbottleSynth is a real-time modal synthesizer model of a 32 Oz.
Wide-Mouth HydroFlask. The amount of water inside the bottle, as well as
the item used to strike the bottle are parameters of the plugin that
change the waterbottle timbre. The swinging vibrato of the waterbottle
can be controlled by the "Swing Damp" and "Swing Modes" knobs. Stickers 
can also be attached to the bottle by clicking and dragging on the screen, 
as well as removed from the bottle by clicking on the surface of the 
sticker. The stickers alter the damping of the waterbottle modes.

### BespokeWaterbottleSynth

<img src="https://raw.githubusercontent.com/jatinchowdhury18/modal-waterbottles/master/Figures/BespokeWaterbottleSynthPlugin.PNG" alt="Pic" width="500">

The BespokeWaterBottleSynth is a real-time synthesizer that can be used
to model any waterbottle. To load your own waterbottle, click the "reload"
button, and select a waterbottle config file (`*.waterbottle`) from your 
computer. As an example, we provide a waterbottle config for the DAFx 2019
waterbottle. To create a `*.waterbottle` file from your own waterbottle 
recording, visit our [web app](ccrmawaterbottles.pythonanywhere.com).

All other parameters are the same as for the WaterbottleSynth above.

## License

The code in this repository is licensed under the GPLv3 license. Enjoy!
