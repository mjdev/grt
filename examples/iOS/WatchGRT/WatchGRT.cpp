/*
 GRT MIT License
 Copyright (c) <2015> <mjahnen>

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * This example can transfer the csv files produced by the WatchGRT example WatchKit app to a GRT trained model.
 */

#include <iostream>
#include <vector>

#include "GRT.h"

using namespace GRT;
using namespace std;

vector<string> split(string s, string delimiter) {
    vector<string> result;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    result.push_back(s);
    return result;
}

int main (int argc, const char * argv[])
{
    if(argc != 4) {
        return 1;
    }

    ifstream input(argv[1]);
    string output(argv[2]);
    int label = atoi(argv[3]);

    GestureRecognitionPipeline pipeline;

    DTW dtw;

    dtw.enableNullRejection(true);
    dtw.enableTrimTrainingData(true, 0.1, 90);
    dtw.setNullRejectionCoeff(3);
    //Offset the timeseries data by the first sample, this makes your gestures (more) invariant to the location the gesture is performed
    //dtw.setOffsetTimeseriesUsingFirstSample(true);

    pipeline.setClassifier(dtw);

    LabelledTimeSeriesClassificationData trainingData;
    trainingData.setNumDimensions(3);

    MatrixDouble timeseries;

    string tmp;
    // skip header
    getline(input, tmp);
    cout << tmp << endl;

    int currentSample = -1;

    while(getline(input, tmp)) {
        vector<string> tokens = split(tmp, ";");

        int sample =  atoi(tokens[0].c_str());

        if(sample != currentSample) {
            cout << "Add new sample to training data" << endl;
            if(timeseries.getNumCols() > 0)
                trainingData.addSample(label, timeseries);
            timeseries.clear();
            currentSample = sample;
        }

        cout << atoi(tokens[0].c_str()) << " " << atof(tokens[1].c_str()) << " " << atof(tokens[1].c_str()) << " " << atof(tokens[2].c_str()) << " " << atof(tokens[3].c_str()) << endl;

        VectorDouble sampleVector(3);
        sampleVector[0] = atof(tokens[1].c_str());
        sampleVector[1] = atof(tokens[2].c_str());
        sampleVector[2] = atof(tokens[3].c_str());
        timeseries.push_back(sampleVector);
    }

    trainingData.save(output + "/" + std::to_string(label) + ".data");

    cout << "Training now" << endl;

    pipeline.train(trainingData);

    pipeline.save(output + "/" + std::to_string(label) + ".model");
}