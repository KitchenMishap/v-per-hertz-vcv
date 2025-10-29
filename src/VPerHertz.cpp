#include "plugin.hpp"

void runTests()
{
	assert(true);
}

const float C4Hz = 256.f;

float VhzFromVoct(float voct, float f0)
{
	float f = pow(2.f, voct) * C4Hz;
	float vhz = f / f0 - 1.f;
	return vhz;
}

float VoctFromVhz(float vhz, float f0)
{
	float f = f0 * (1.f + vhz);
	float voct = (log(f) - log(256.f)) / log(2.f);
    return voct;
}

struct VPerHertz : Module {
	enum ParamId {
		MANUAL_PARAM,
		CALIBRATE_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		VHZ1_INPUT,
		VHZ2_INPUT,
		VOCT1_INPUT,
		VOCT2_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		VOCT1_OUTPUT,
		VOCT2_OUTPUT,
		VHZ1_OUTPUT,
		VHZ2_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		CALIBRATED_LIGHT,
		LIGHTS_LEN
	};

	float oldCalButton = 0.f;

	VPerHertz() {
		INFO("ToDo: Running Tests...");
		runTests();
		INFO("ToDo: ...Tests completed");

		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		//configSwitch(TODO_PARAM, 0.f, 2.f, 0.f, "ToDo Param", {"0.0", "1.0", "2.0"});
		configButton(CALIBRATE_PARAM, "Calibrate");
		configParam(MANUAL_PARAM, 10.f, 10000.f, 1000.f, "f_0 (Hz)" );
		configInput(VOCT1_INPUT, "V/Oct 1");
		configOutput(VHZ1_OUTPUT, "V/Hz 1");
		configInput(VOCT2_INPUT, "V/Oct 2");
		configOutput(VHZ2_OUTPUT, "V/Hz 2");

		oldCalButton = 0.f;
	}


	void process(const ProcessArgs& args) override {
		float calButton = params[CALIBRATE_PARAM].getValue();
		if( calButton == 1.f && oldCalButton == 0.f )
		{
			// Perform calibration
			float vhz = inputs[VHZ1_INPUT].getVoltage();
			if( vhz > 0.f )
			{
				float f0 = 256.f / (1.f + vhz);
				params[MANUAL_PARAM].setValue(f0);
				lights[CALIBRATED_LIGHT].setBrightness(1.f);
			}
		}
		oldCalButton = calButton;

		float f0 = params[MANUAL_PARAM].getValue();
		float vhz = inputs[VHZ1_INPUT].getVoltage();
		float voct = VoctFromVhz(vhz, f0);
		outputs[VOCT1_OUTPUT].setVoltage(voct);

		vhz = inputs[VHZ2_INPUT].getVoltage();
		voct = VoctFromVhz(vhz, f0);
		outputs[VOCT2_OUTPUT].setVoltage(voct);

		voct = inputs[VOCT1_INPUT].getVoltage();
		vhz = VhzFromVoct(voct, f0);
		outputs[VHZ1_OUTPUT].setVoltage(vhz);

		voct = inputs[VOCT2_INPUT].getVoltage();
		vhz = VhzFromVoct(voct, f0);
		outputs[VHZ2_OUTPUT].setVoltage(vhz);
		}
};

struct VPerHertzWidget : ModuleWidget {
	VPerHertzWidget(VPerHertz* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/GeneratedPanelPaths.svg")));

		float width = 5.08 * 6;
		float col1 = 5.08 * 1.5;
		float col2 = 5.08 * 4.5;

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec((col1+col2)/2.f, 25)), module, VPerHertz::MANUAL_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(col1, 55)), module, VPerHertz::CALIBRATE_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7, 101-29)), module, VPerHertz::VHZ1_INPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(width - 7, 101-29)), module, VPerHertz::VOCT1_OUTPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7, 113-29)), module, VPerHertz::VHZ2_INPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(width - 7, 113-29)), module, VPerHertz::VOCT2_OUTPUT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7, 101)), module, VPerHertz::VOCT1_INPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(width - 7, 101)), module, VPerHertz::VHZ1_OUTPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7, 113)), module, VPerHertz::VOCT2_INPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(width - 7, 113)), module, VPerHertz::VHZ2_OUTPUT));

		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(width - 7, 54)), module, VPerHertz::CALIBRATED_LIGHT));

		// mm2px(Vec(10.0, 10.0))
		addChild(createWidget<Widget>(mm2px(Vec(2.684, 81.945))));
	}
};

Model* modelVPerHertz = createModel<VPerHertz, VPerHertzWidget>("VPerHertz");