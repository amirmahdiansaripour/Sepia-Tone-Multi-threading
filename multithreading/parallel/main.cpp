#include "../filters/def.h"

long NUMBER_OF_THREADS;
long MAX_NUMBER_OF_THREADS = 200;
vector<ImageThread> imageThreads;

void* threadHandler(void* threadId){
  long index = (long)threadId;
  sepia(imageThreads[index]);
  sepia(imageThreads[index]);
  sepia(imageThreads[index]);
  pthread_exit(NULL);
}


void setThreadDimensions(Image* image){
  int rows = image->pixcels.size();
  int columns = image->pixcels[0].size();
  // cout << "rows\t" << rows << "\tcolumns\t" << columns << "\n"; 
  if(rows % NUMBER_OF_THREADS != 0){
    cout << "Can not divide image to equal-width segments\n";
    exit(0);
  }
  int offset = rows / NUMBER_OF_THREADS;
  int counter = 0;
  for(int i = 0; i <= rows - 1; i += offset){
    imageThreads[counter].firstRow = i;
    imageThreads[counter].lastRow = i + offset;
    imageThreads[counter].firstColumn = 0;
    imageThreads[counter].lastColumn = columns - 1;
    imageThreads[counter].imagePointingTo = new Image;
    imageThreads[counter].imagePointingTo = image;
    imageThreads[counter].unprocessedImage = image->pixcels;
    counter++;
  }

  return;
}

void handleThreads(Image* image){
  int created, joined;
  for(long i = 0; i < NUMBER_OF_THREADS; i++){
    pthread_create(&imageThreads[i].thread, NULL, threadHandler, (void*)i);
  }
  for(long j = 0; j < NUMBER_OF_THREADS; j++){
    pthread_join(imageThreads[j].thread, NULL);
  }
}

vector<long> getDivedends(int imageSize){
  vector<long>dividends;
  for(int numOfthreads = 2; numOfthreads < imageSize; numOfthreads++){
    if(imageSize % numOfthreads == 0 && numOfthreads <= MAX_NUMBER_OF_THREADS)
      dividends.emplace_back(numOfthreads);
  }
  return dividends;
}

vector<float> runParallel(Image* image, char *fileBuffer, int bufferSize){
  // float rate = (float) NUMBER_OF_THREADS;
  auto start = high_resolution_clock::now();
  setThreadDimensions(image);
  handleThreads(image);
  writeOutBmp24(fileBuffer, FILE_TO_PUT_THE_OUTPUT + to_string(NUMBER_OF_THREADS) + "threads.bmp", bufferSize, *image);
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);
  // cout << "Time spent for: "  << NUMBER_OF_THREADS << "\t threads is "<< duration.count() << "\n";
  return {(float) NUMBER_OF_THREADS, (float)duration.count()};
}

void writeAllSamplesToCSV(const vector<vector<float>>& samples, float optimizedNumOfThreads){
  ofstream recordsFile;
  recordsFile.open(CSV_FILE);
  recordsFile << "x,y\n";
  recordsFile << optimizedNumOfThreads << "\n";
  for(vector<float> row : samples){
    recordsFile << row[0] << "," << row[1] << "\n";
  }
  recordsFile.close();
}

float getOptimizedNumberOfThreads(const vector<vector<float>>& samples){
  float optimizedNumberOfThreads = samples[0][0];
  float minTimeSpent = samples[0][1];
  for(int i = 0; i < samples.size(); i++){
    if(samples[i][1] < minTimeSpent){
      minTimeSpent = samples[i][1];
      optimizedNumberOfThreads = samples[i][0];
    }
  }
  return optimizedNumberOfThreads;
}


void writeMinSampleToCSV(float optimizedNumberOfThreads){
  fstream minValueRecord;
  minValueRecord.open(MIN_CSV_FILE, ios::out | ios::app);
  minValueRecord << optimizedNumberOfThreads << '\n';
  minValueRecord.close();
}

void writeToCSV(const char *runSingleOrHundred, const vector<vector<float>> &sampleOutput){
  string temp = runSingleOrHundred;
  float optimizedNumberOfThreads = getOptimizedNumberOfThreads(sampleOutput);
  if(temp == "oneTime"){

    writeAllSamplesToCSV(sampleOutput, optimizedNumberOfThreads);
  }
  else if(temp == "hundredTimes"){
    writeMinSampleToCSV(optimizedNumberOfThreads);
  }
  else{
    cout << "Wrong argument \n\n";    
  }
}

int main(int argc, char *argv[]){
  char *fileName = argv[1];  
  char* runSingleOrHundred = argv[2];
  char *temporary = readBMP24(fileName); 
  vector<int> fileDimensions = getFileSize(temporary);  
  int rows = fileDimensions[0];
  int columns = fileDimensions[1];
  int bufferSize = fileDimensions[2];

  vector<long> feasibleNumOfThreads = getDivedends(rows);
  vector<vector<float>> sampleOutput;
  for(int i = 0; i < feasibleNumOfThreads.size(); i++){
    char *fileBuffer = readBMP24(fileName);
    NUMBER_OF_THREADS = feasibleNumOfThreads[i];
    Image* image = new Image;
    imageThreads = vector<ImageThread>(NUMBER_OF_THREADS);
    image->pixcels = vector<vector<Pixcel>>(rows, vector<Pixcel>(columns));
    getPixlesFromBMP24(bufferSize, fileBuffer, *image);
    sampleOutput.emplace_back(runParallel(image, fileBuffer, bufferSize));
    free(image); 
    free(fileBuffer); 
  }
  writeToCSV(runSingleOrHundred, sampleOutput);
  free(temporary);
  return 0;
}