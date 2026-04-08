#include "DHT.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "model_data.h"  // Include the trained model

// Define DHT22 sensor pin
#define DHTPIN 2  // Change this to the actual pin you're using
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// TensorFlow Lite Model Variables
const tflite::Model* model;
tflite::MicroInterpreter* interpreter;
TfLiteTensor* input;
TfLiteTensor* output;

constexpr int kTensorArenaSize = 8000;
uint8_t tensor_arena[kTensorArenaSize];

void setup() {
    Serial.begin(115200);
    dht.begin();  // Initialize DHT sensor
    Serial.println("Initializing TensorFlow Lite Micro...");

    // Load the model
    model = tflite::GetModel(temperature_prediction_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model schema version mismatch!");
        return;
    }

    // Add required operations
    static tflite::MicroMutableOpResolver<6> resolver;  // Increase the resolver size
    resolver.AddFullyConnected();
    resolver.AddSoftmax();
    resolver.AddRelu();
    resolver.AddReshape();
    resolver.AddQuantize();
    resolver.AddLogistic();  // ✅ Add the missing LOGISTIC function



    // Create the interpreter
    static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize);
    interpreter = &static_interpreter;

    // Allocate memory for model tensors
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        Serial.println("Tensor allocation failed!");
        return;
    }

    // Get input and output tensors
    input = interpreter->input(0);
    output = interpreter->output(0);

    Serial.println("Setup complete.");
}

void loop() {
    delay(2000);
    
    float temperature = dht.readTemperature();  // Read temperature from DHT22

    if (isnan(temperature)) {
        Serial.println("Failed to read temperature!");
        return;
    }

    Serial.print("Measured Temperature: ");
    Serial.println(temperature);

    // Provide input to TensorFlow Lite model
    input->data.f[0] = temperature;

    // Run the model inference
    if (interpreter->Invoke() != kTfLiteOk) {
        Serial.println("Inference failed!");
        return;
    }

    // Interpret output (assuming classification: "Normal Temp" = 0, "Bad Temp" = 1)
    float result = output->data.f[0];

    Serial.print("Prediction: ");
    if (result > 0.5) {
        Serial.println("⚠️ Bad Temperature! Take action!");
    } else {
        Serial.println("✅ Normal Temperature.");
    }

    delay(2000);  // Read data every 2 seconds
}
