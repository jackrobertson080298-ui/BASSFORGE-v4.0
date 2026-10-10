#include "PluginEditor.h"
BassforgeEditor::BassforgeEditor(BassforgeProcessor& p):AudioProcessorEditor(&p),processor(p){
 setSize(850,700);
 synthTab.onClick=[this]{showFx=false;updatePage();};fxTab.onClick=[this]{showFx=true;updatePage();};addAndMakeVisible(synthTab);addAndMakeVisible(fxTab);
 title.setText("BASSFORGE",juce::dontSendNotification);title.setFont(juce::Font(juce::FontOptions(32.f,juce::Font::bold)));title.setColour(juce::Label::textColourId,juce::Colours::white);addAndMakeVisible(title);
 subtitle.setText("DNB BASS SYNTH  /  v0.4",juce::dontSendNotification);subtitle.setColour(juce::Label::textColourId,juce::Colour(0xffa0b0bf));addAndMakeVisible(subtitle);
 for(int i=0;i<30;++i)presets.addItem(factoryPatches[(size_t)i].name,i+1);
 presets.setSelectedId(p.getCurrentProgram()+1,juce::dontSendNotification);
 presets.onChange=[this]{processor.setCurrentProgram(presets.getSelectedId()-1);};addAndMakeVisible(presets);
 const char* ids[]={"drive","fm","detune","cutoff","movement","release","tone","width","sub","output","punch","rasp"};
 for(int i=0;i<12;++i){auto& s=knobs[(size_t)i];s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);s.setTextBoxStyle(juce::Slider::TextBoxBelow,false,65,19);s.setColour(juce::Slider::rotarySliderFillColourId,juce::Colour(0xff39d8bf));s.setColour(juce::Slider::thumbColourId,juce::Colours::white);addAndMakeVisible(s);auto& l=labels[(size_t)i];l.setText(juce::String(ids[i]).toUpperCase(),juce::dontSendNotification);l.setJustificationType(juce::Justification::centred);l.setColour(juce::Label::textColourId,juce::Colours::white);addAndMakeVisible(l);attachments[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,ids[i],s);}
 shape.addItemList({"Saw","Horn","Metal"},1);addAndMakeVisible(shape);shapeAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.params,"shape",shape);
 keyboard.setAvailableRange(24,96);addAndMakeVisible(keyboard);
 bypassAttach=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.params,"fxbypass",bypass);addAndMakeVisible(bypass);
 for(int i=0;i<7;++i){
  auto id="fx"+juce::String(i);
  addAndMakeVisible(fxLabels[(size_t)i]);addAndMakeVisible(fxEnabled[(size_t)i]);
  addAndMakeVisible(fxMix[(size_t)i]);addAndMakeVisible(fxAmount[(size_t)i]);
  auto setup=[](juce::Slider& sl){sl.setSliderStyle(juce::Slider::LinearHorizontal);sl.setTextBoxStyle(juce::Slider::TextBoxRight,false,55,20);};
  setup(fxMix[(size_t)i]);setup(fxAmount[(size_t)i]);
  fxEnabledAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.params,id+"on",fxEnabled[(size_t)i]);
  fxMixAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,id+"mix",fxMix[(size_t)i]);
  fxAmountAttach[(size_t)i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.params,id+"amt",fxAmount[(size_t)i]);
  fxUp[(size_t)i].setButtonText("UP");fxDown[(size_t)i].setButtonText("DOWN");
  fxUp[(size_t)i].onClick=[this,i]{for(int j=0;j<7;++j)if(processor.getFxAt(j)==i){processor.moveFx(j,-1);break;}refreshFxLabels();};
  fxDown[(size_t)i].onClick=[this,i]{for(int j=0;j<7;++j)if(processor.getFxAt(j)==i){processor.moveFx(j,1);break;}refreshFxLabels();};
  addAndMakeVisible(fxUp[(size_t)i]);addAndMakeVisible(fxDown[(size_t)i]);
 }
 refreshFxLabels();updatePage();
}
void BassforgeEditor::refreshFxLabels(){
 for(int i=0;i<7;++i){int fx=i;fxLabels[(size_t)i].setText(juce::String([this,i]{for(int j=0;j<7;++j)if(processor.getFxAt(j)==i)return j+1;return i+1;}())+"  "+BassforgeProcessor::fxNames[fx],juce::dontSendNotification);}
}
void BassforgeEditor::updatePage(){
 for(auto& x:knobs)x.setVisible(!showFx);for(auto& x:labels)x.setVisible(!showFx);shape.setVisible(!showFx);
 for(int i=0;i<7;++i){fxLabels[(size_t)i].setVisible(showFx);fxEnabled[(size_t)i].setVisible(showFx);fxMix[(size_t)i].setVisible(showFx);fxAmount[(size_t)i].setVisible(showFx);fxUp[(size_t)i].setVisible(showFx);fxDown[(size_t)i].setVisible(showFx);}
 bypass.setVisible(showFx);repaint();
}
void BassforgeEditor::paint(juce::Graphics& g){g.fillAll(juce::Colour(0xff101821));g.setColour(juce::Colour(0xff253544));g.fillRoundedRectangle(16.f,140.f,818.f,450.f,12.f);g.setColour(juce::Colour(0xff39d8bf));g.fillRect(20,90,810,2);}
void BassforgeEditor::resized(){
 title.setBounds(24,15,290,45);subtitle.setBounds(26,59,310,23);presets.setBounds(480,25,342,32);
 synthTab.setBounds(25,98,110,32);fxTab.setBounds(143,98,110,32);
 shape.setBounds(650,65,172,27);
 for(int i=0;i<12;++i){const int x=35+(i%5)*162,y=154+(i/5)*110;knobs[(size_t)i].setBounds(x,y,125,91);labels[(size_t)i].setBounds(x,y+91,125,24);}
 bypass.setBounds(655,105,165,27);
 for(int i=0;i<7;++i){int y=156+i*59;fxLabels[(size_t)i].setBounds(30,y,165,24);fxEnabled[(size_t)i].setBounds(199,y,28,26);fxMix[(size_t)i].setBounds(238,y,205,27);fxAmount[(size_t)i].setBounds(452,y,205,27);fxUp[(size_t)i].setBounds(665,y,70,27);fxDown[(size_t)i].setBounds(739,y,78,27);}
 keyboard.setBounds(22,615,806,68);
}
juce::AudioProcessorEditor* BassforgeProcessor::createEditor(){return new BassforgeEditor(*this);}
