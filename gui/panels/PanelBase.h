
#pragma once

#include <map>
#include <functional>

#include "JuceHeader.h"
#include "SynthParams.h"
#include "MouseOverKnob.h"
#include "IncDecDropDown.h"

class PanelBase : public Component, protected Timer
{
public:

    PanelBase(SynthParams &p)
        : params(p)
    {
        startTimerHz(60);
    }

    ~PanelBase() {
        stopTimer();
    }

    static const int COMBO_OFS = 2;
protected:
    typedef std::function<void()> tHookFn;

    void registerSlider(Slider *slider, Param *p, const tHookFn hook = tHookFn()) {
        sliderReg[slider] = p;
        if (hook) {
            postUpdateHook[slider] = hook;
        }
        if (p->hasLabels()) {
            slider->setName(p->getUIString());
        }
        else {
            slider->setName(p->name());
            slider->setTextValueSuffix(String(" ") + p->unit());
        }
        slider->setValue(p->getUI());
    }

    void registerSlider(MouseOverKnob *slider, Param *p, const tHookFn hook = tHookFn()) {
        slider->setDefaultValue(p->getUI());
        registerSlider(static_cast<Slider*>(slider), p);
        if (hook) {
            postUpdateHook[slider] = hook;
        }
        slider->initTextBox();
    }

    // NOTE: sourceNumber values 1 or 2
    // TODO: change it to an enum?
    void registerSaturnSource(MouseOverKnob *dest, Slider *source, ParamStepped<eModSource> *modSource, Param *modAmount, int sourceNumber
        , MouseOverKnob::modAmountConversion convType = MouseOverKnob::modAmountConversion::noConversion) {
        dest->setModSource(modSource, modAmount, sourceNumber, convType);

        auto temp = saturnReg.find(dest);
        if (temp == saturnReg.end()) {
            std::array<Slider*, 2> newSource = {nullptr};
            newSource[sourceNumber-1] = source;
            saturnReg[dest] = newSource;
        } else {
            temp->second[sourceNumber-1] = source;
        }
    }

    void updateDirtySliders() {
        for (auto s2p : sliderReg) {
            if (s2p.second->isUIDirty()) {
                s2p.first->setValue(s2p.second->getUI());
                if (s2p.second->hasLabels()) {
                    s2p.first->setName(s2p.second->getUIString());
                }

                auto itHook = postUpdateHook.find(s2p.first);
                if (itHook != postUpdateHook.end()) {
                    itHook->second();
                }
            }

        }
    }

    // Callback function in case there are any dirty saturn glows
    void updateDirtySaturns() {
        // iterate over all the registered saturn glows
        for (auto dest2saturn : saturnReg) {
            // find the mod source from the slider register
            for (int i = 0; i < 2; ++i) {
                auto modSource = sliderReg.find(dest2saturn.second[i]);

                //if the mod source is Dirty repaint
                if (modSource != sliderReg.end() && modSource->second->isUIDirty()) {
                    dest2saturn.first->repaint();
                }
            }
        }
    }

    void updateDirtyBoxes() {
        for (auto c2p : comboboxReg) {
            if (c2p.second->isUIDirty()) {
                c2p.first->setSelectedId(static_cast<int>(c2p.second->getStep()) + COMBO_OFS);

                auto c2s = saturnSourceReg.find(c2p.first);
                if (c2s != saturnSourceReg.end()) {
                    for (int i = 0; i < c2s->second.size(); ++i ) {
                        if (c2s->second[i] != nullptr) {
                            c2s->second[i]->repaint();
                        }
                    }

                }

                auto itHook = postUpdateHook.find(c2p.first);
                if (itHook != postUpdateHook.end()) {
                    itHook->second();
                }
            }
        }

    }

    // TODO: Change for ParamStepped? It might be just useful for the notelength, so maybe a general solution should be better.
    void updateDirtyDropdowns()
    {
        for (auto d2p : dropdownReg) {
            if (d2p.second->isUIDirty()) {
                d2p.first->setText("1/" + String(d2p.second->getUI()));
            }

            auto itHook = postUpdateHook.find(d2p.first);
            if (itHook != postUpdateHook.end()) {
                itHook->second();
            }
        }
    }

    bool handleSlider(Slider* sliderThatWasMoved) {
        auto it = sliderReg.find(sliderThatWasMoved);
        if (it != sliderReg.end()) {
            it->second->setUI(static_cast<float>(it->first->getValue()));
            if (it->second->hasLabels()) {
                it->first->setName(it->second->getUIString());
            }

            for (auto saturn : saturnReg) {
                for (int i = 0; i < 2; ++i) {
                    if (saturn.second[i] == sliderThatWasMoved) {
                        saturn.first->repaint();
                    }
                }
            }

            auto itHook = postUpdateHook.find(it->first);
            if (itHook != postUpdateHook.end()) {
                itHook->second();
            }

            return true;
        }
        else {
            return false;
        }
    }
    
    void registerToggle(Button* toggle, ParamStepped<eOnOffToggle>* p, const tHookFn hook = tHookFn())
    {
        toggleReg[toggle] = p;
        
        if (hook) {
            postUpdateHook[toggle] = hook;
        }
    }
    
    bool handleToggle(Button* buttonThatWasClicked)
    {
        auto it = toggleReg.find(buttonThatWasClicked);
        
        if (it != toggleReg.end()) {
            it->second->setStep(it->first->getToggleState() ? eOnOffToggle::eOn : eOnOffToggle::eOff);
            
            auto itHook = postUpdateHook.find(it->first);
            if (itHook != postUpdateHook.end()) {
                itHook->second();
            }
            
            return true;
        }
        
        return false;
    }
    
    void updateDirtyToggles()
    {
        for (auto t2p : toggleReg) {
            if (t2p.second->isUIDirty()) {
                t2p.first->setToggleState(static_cast<bool>(t2p.second->getStep()), dontSendNotification);
                
                auto itHook = postUpdateHook.find(t2p.first);
                if (itHook != postUpdateHook.end()) {
                    itHook->second();
                }
            }
        }
    }

    // TODO: Change for ParamStepped? It might be just useful for the notelength, so maybe a general solution should be better.
    void registerDropdown(ComboBox* dropdown, Param* p, const tHookFn hook = tHookFn())
    {
        dropdownReg[dropdown] = p;

        if (hook) {
            postUpdateHook[dropdown] = hook;
        }
    }

    // TODO: Change for ParamStepped?
    bool handleDropdown(ComboBox* dropdownThatWasChanged)
    {
        auto it = dropdownReg.find(dropdownThatWasChanged);

        if (it != dropdownReg.end()) {
            it->second->setUI(static_cast<float>(std::pow(2,it->first->getSelectedItemIndex())));
            return true;
        }
        else
        {
            return false;
        }
    }

    void registerCombobox(ComboBox* box, ParamStepped<eModSource> *p, std::array<MouseOverKnob*, 3> modDest = {nullptr}, const tHookFn hook = tHookFn()) {
        comboboxReg[box] = p;

        // couple combobox with saturn knob
        if (!modDest.empty()) {
            saturnSourceReg[box] = modDest;
        }

        box->setSelectedId(static_cast<int>(p->getStep())+COMBO_OFS);

        if (hook) {
            postUpdateHook[box] = hook;
        }
    }

    bool handleCombobox(ComboBox* comboboxThatWasChanged) {
        auto it = comboboxReg.find(comboboxThatWasChanged);
        if (it != comboboxReg.end()) {
            // we gotta subtract 2 from the item id since the combobox ids start at 1 and the sources enum starts at -1
            params.globalModMatrix.changeSource(it->second->prefix()+" "+comboboxThatWasChanged->getName(), static_cast<eModSource>(comboboxThatWasChanged->getSelectedId() - COMBO_OFS));
            // we gotta subtract 1 from the item id since the combobox ids start at 1 and the eModSources enum starts at 0
            it->second->setStep(static_cast<eModSource>(it->first->getSelectedId() - COMBO_OFS));

            if (it->second->getStep() == eModSource::eNone)
            {
                it->first->setColour(ComboBox::ColourIds::backgroundColourId, it->first->findColour(ComboBox::ColourIds::backgroundColourId).withAlpha(0.5f));
            }
            else
            {
                it->first->setColour(ComboBox::ColourIds::backgroundColourId, it->first->findColour(ComboBox::ColourIds::backgroundColourId).withAlpha(1.0f));
            }
            it->first->setColour(ComboBox::ColourIds::textColourId, SynthParams::getModSourceColour(static_cast<eModSource>(it->first->getSelectedId() - COMBO_OFS)));
            it->first->setText(SynthParams::getShortModSrcName(it->first->getSelectedId() - COMBO_OFS), dontSendNotification);

            auto temp = saturnSourceReg.find(comboboxThatWasChanged);

            // update saturn
            if (temp != saturnSourceReg.end()) {
                for (int i = 0; i < temp->second.size(); ++i ) {
                    if (temp->second[i] != nullptr) {
                        temp->second[i]->repaint();
                    }
                }
            }

            return true;
        }
        else {
            return false;
        }
    }

    void fillModsourceBox(ComboBox* box) {
        for (int i = eModSource::eNone; i < eModSource::nSteps; i++) {
            box->addItem(params.getModSrcName(i), i + COMBO_OFS);
        }
    }

    virtual void timerCallback() override {
        updateDirtySaturns();
        updateDirtySliders();
        updateDirtyBoxes();
        updateDirtyDropdowns();
        updateDirtyToggles();
    }

    /**
    * Draw white group border with group name alligned right.
    */
    void drawGroupBorder(Graphics &g, const String &name, int x, int y, int width, int height, float headHeight, float cornerSize, float borderThickness, float padding, Colour c)
    {
        float posX = static_cast<float>(x) + padding;
        float posY = static_cast<float>(y) + padding;
        float boxWidth = static_cast<float>(width) - 2.0f * padding;
        float boxHeight = static_cast<float>(height) - 2.0f * padding;

        // draw white groupborder
        Rectangle<float> rect = { posX, posY, boxWidth, boxHeight };
        g.setColour(Colours::white);
        g.fillRoundedRectangle(rect, cornerSize);

        rect = { posX + borderThickness, posY + headHeight, boxWidth - borderThickness * 2.0f, boxHeight - headHeight - borderThickness };
        g.setColour(c);
        g.fillRoundedRectangle(rect, cornerSize);

        // draw group name text
        int offset = 2 * static_cast<int>(cornerSize);
        g.setFont(headHeight * 0.85f);
        g.drawText(name, static_cast<int>(posX) + offset, static_cast<int>(posY),
            width - 2 * offset, static_cast<int>(posY) + static_cast<int>(headHeight - (headHeight - headHeight * 0.85f) * 0.5f), Justification::centredRight);
    }

    std::map<Slider*, Param*> sliderReg;
    std::map<Button*, ParamStepped<eOnOffToggle>*> toggleReg;
    std::map<ComboBox*, ParamStepped<eModSource>*> comboboxReg;
    std::map<Component*, tHookFn> postUpdateHook;
    std::map<ComboBox*, Param*> dropdownReg;
    std::map<MouseOverKnob*, std::array<Slider*, 2>> saturnReg;
    std::map<ComboBox*, std::array<MouseOverKnob*, 3>> saturnSourceReg; // there are 3, because of the ADR.
    SynthParams &params;
};
