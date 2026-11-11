#include <juce_audio_processors/juce_audio_processors.h>

class LRProcessor : public juce::AudioProcessor
{
public:
    LRProcessor()
        : AudioProcessor(BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    {
        addParameter(side = new juce::AudioParameterChoice(
            juce::ParameterID{ "side", 1 }, "L/R",
            juce::StringArray{ "Left", "Right" }, 0));
    }

    const juce::String getName() const override { return "L_R"; }

    void prepareToPlay(double, int) override {}
    void releaseResources() override {}

    bool isBusesLayoutSupported(const BusesLayout& l) const override
    {
        return l.getMainInputChannelSet() == juce::AudioChannelSet::stereo()
            && l.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override
    {
        if (buffer.getNumChannels() < 2)
            return;

        auto* l = buffer.getWritePointer(0);
        auto* r = buffer.getWritePointer(1);
        const int n = buffer.getNumSamples();

        if (side->getIndex() == 0)
            juce::FloatVectorOperations::copy(r, l, n);   // right = left
        else
            juce::FloatVectorOperations::copy(l, r, n);   // left = right
    }

    bool hasEditor() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& dest) override
    {
        juce::MemoryOutputStream s(dest, true);
        s.writeInt(side->getIndex());
    }

    void setStateInformation(const void* data, int size) override
    {
        juce::MemoryInputStream s(data, (size_t) size, false);
        if (size >= 4)
            *side = juce::jlimit(0, 1, s.readInt());
    }

private:
    juce::AudioParameterChoice* side = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LRProcessor)
};

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LRProcessor();
}
