#include "plugin.hpp"

void runTests()
{
	assert(true);
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
		CALIBRATEDD_LIGHT,
		LIGHTS_LEN
	};

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
	}


	void process(const ProcessArgs& args) override {
		// Example: Set polyphonic output equal to input
		//int numPoly = inputs[TODO_INPUT].getChannels();
		//outputs[TODO_OUTPUT].setChannels(numPoly);
		//for( int i=0; i<numPoly; i++ ) {
		//	float inputV = inputs[TODO_INPUT].getVoltage(i);
		//	outputs[TODO_OUTPUT].setVoltage(inputV, i);
		//}
		// Example: Turn LED on
		//lights[TODO_LIGHT].setBrightness(1.f);
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

		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(width - 7, 54)), module, VPerHertz::CALIBRATEDD_LIGHT));

		// mm2px(Vec(10.0, 10.0))
		addChild(createWidget<Widget>(mm2px(Vec(2.684, 81.945))));
	}
};

Model* modelVPerHertz = createModel<VPerHertz, VPerHertzWidget>("VPerHertz");