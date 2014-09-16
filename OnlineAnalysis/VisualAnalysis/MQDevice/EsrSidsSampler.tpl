/* 
 * File:   EsrSidsSampler.tpl
 * Author: winckler
 *
 * Created on August 6, 2014, 3:06 PM
 */


template <typename TSamplerTask>
EsrSidsSampler<TSamplerTask>::EsrSidsSampler() :
      //fSpSamplerTask(NULL),
      fInputFile(""),
      fParFile(""),
      fBranch(""),
      fEventRate(1),
      fNumEvents(0)
{
}


template <typename TSamplerTask>
EsrSidsSampler<TSamplerTask>::~EsrSidsSampler()
{
    //if(fSpSamplerTask)
      //  delete fSpSamplerTask;
}


template <typename TSamplerTask>
void EsrSidsSampler<TSamplerTask>::SetTask(std::shared_ptr<TSamplerTask> task)
{
  fSpSamplerTask = task;
}

/// INIT
template <typename TSamplerTask>
void EsrSidsSampler<TSamplerTask>::Init()
{
    FairMQDevice::Init();
    fSpSamplerTask->SetTransport(fTransportFactory);
    fSpSamplerTask->SetInputFile(fInputFile);
    fNumEvents=fSpSamplerTask->GetNumEvents();
    
    
    fSpSamplerTask->InitTask();
    
}

template <typename TSamplerTask>
void EsrSidsSampler<TSamplerTask>::Run()
{
    LOG(INFO) << ">>>>>>> Run <<<<<<<";
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    boost::thread rateLogger(boost::bind(&FairMQDevice::LogSocketRates, this));
    boost::thread resetEventCounter(boost::bind(&EsrSidsSampler::ResetEventCounter, this));

    int sentMsgs = 0;
    boost::timer::auto_cpu_timer timer;
    cout << "Number of events to process: " << fNumEvents << endl;

    unsigned int eventNr = 0;
if( fState != RUNNING ) 
    cout << "Not running "  << endl;
else cout << " running "  << endl;
    
    //*
    fSpSamplerTask->SetEventIndex(eventNr);
    fSpSamplerTask->ExecuteTask("");

    fPayloadOutputs->at(0)->Send(fSpSamplerTask->GetOutput());
    sentMsgs++;
    //*/
    /*
    while (fState == RUNNING) 
    {
        //if(fSpSamplerTask->ReadyToSend())
        //{
            fSpSamplerTask->SetEventIndex(eventNr);
            fSpSamplerTask->ExecuteTask("");

            fPayloadOutputs->at(0)->Send(fSpSamplerTask->GetOutput());
            sentMsgs++;

            --fEventCounter;

            while (fEventCounter == 0) 
            {
                boost::this_thread::sleep(boost::posix_time::milliseconds(1));
            }

            if( fState != RUNNING ) 
                break;
            break;
        //}
        //break;
    }
    // */
    boost::this_thread::interruption_point();
    boost::timer::cpu_times const elapsed_time(timer.elapsed());

    cout << "Sent everything in:\n" << boost::timer::format(elapsed_time, 2) << endl;
    cout << "Sent " << sentMsgs << " messages!" << endl;

    try 
    {
        rateLogger.interrupt();
        rateLogger.join();
        // resetEventCounter.interrupt();
        // resetEventCounter.join();
        // commandListener.interrupt();
        // commandListener.join();
    }
    catch (boost::thread_resource_error &e) 
    {
        LOG(ERROR) << e.what();
    }
    
    FairMQDevice::Shutdown();
    
    boost::lock_guard<boost::mutex> lock(fRunningMutex);
    fRunningFinished = true;
    fRunningCondition.notify_one();
    
    
}



template <typename TSamplerTask>
void EsrSidsSampler<TSamplerTask>::ResetEventCounter()
{
  while ( true ) {
    try {
      fEventCounter = fEventRate / 100;
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    } catch (boost::thread_interrupted&) {
      cout << "resetEventCounter interrupted" << endl;
      break;
    }
  }
  LOG(DEBUG) << ">>>>>>> stopping resetEventCounter <<<<<<<";
}

template <typename TSamplerTask>
void EsrSidsSampler<TSamplerTask>::ListenToCommands()
{
  LOG(INFO) << ">>>>>>> ListenToCommands <<<<<<<";

  bool received = false;

  while ( true ) {
    try {
      FairMQMessage* msg = fTransportFactory->CreateMessage();

      received = fPayloadInputs->at(0)->Receive(msg);

      if (received) {
        //command handling goes here.
        LOG(INFO) << "> received command <";
        received = false;
      }

      delete msg;

      boost::this_thread::interruption_point();
    } catch (boost::thread_interrupted&) {
      cout << "commandListener interrupted" << endl;
      break;
    }
  }
  LOG(DEBUG) << ">>>>>>> stopping commandListener <<<<<<<";
}

template <typename TSamplerTask>
void EsrSidsSampler<TSamplerTask>::SetProperty(const int key, const string& value, const int slot/*= 0*/)
{
  switch (key) {
  case InputFile:
    fInputFile = value;
    break;
  case ParFile:
    fParFile = value;
    break;
  case Branch:
    fBranch = value;
    break;
  default:
    FairMQDevice::SetProperty(key, value, slot);
    break;
  }
}

template <typename TSamplerTask>
string EsrSidsSampler<TSamplerTask>::GetProperty(const int key, const string& default_/*= ""*/, const int slot/*= 0*/)
{
  switch (key) {
  case InputFile:
    return fInputFile;
  case ParFile:
    return fParFile;
  case Branch:
    return fBranch;
  default:
    return FairMQDevice::GetProperty(key, default_, slot);
  }
}

template <typename TSamplerTask>
void EsrSidsSampler<TSamplerTask>::SetProperty(const int key, const int value, const int slot/*= 0*/)
{
  switch (key) {
  case EventRate:
    fEventRate = value;
    break;
  default:
    FairMQDevice::SetProperty(key, value, slot);
    break;
  }
}

template <typename TSamplerTask>
int EsrSidsSampler<TSamplerTask>::GetProperty(const int key, const int default_/*= 0*/, const int slot/*= 0*/)
{
  switch (key) {
  case EventRate:
    return fEventRate;
  default:
    return FairMQDevice::GetProperty(key, default_, slot);
  }
}



