using System;
using System.Collections.Generic;
using System.Threading;
using System.Windows;

namespace Monitor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Logger logger;
        private List<Thread> threads;

        public MainWindow()
        {
            InitializeComponent();
            logger = new Logger("log.txt");
            threads = new List<Thread>();
        }

        private void StartButtonClick(object sender, RoutedEventArgs e)
        {


            int threadCount;
            if (int.TryParse(threadCountTextBox.Text, out threadCount) && threadCount >= 5 && threadCount <= 20)
            {
                StartThreads(threadCount);
            }
            else
            {
                MessageBox.Show("Please enter a valid thread count between 5 and 20.");
            }
        }

        private void StartThreads(int threadCount)
        {
            for (int i = 0; i < threadCount; i++)
            {
                Thread thread = new Thread(ThreadMethod);
                threads.Add(thread);
                thread.Start(i + 1);
            }
        }

        private void ThreadMethod(object threadNumberObj)
        {
            int threadNumber = (int)threadNumberObj;
            Random random = new Random();

            int randomDelay = random.Next(1000); 
            Thread.Sleep(randomDelay);

            DateTime currentTime = DateTime.Now;
            string logMessage = $"Thread {threadNumber}: {currentTime:HH:mm:ss.fff} - Delay: {randomDelay} ms";

            System.Threading.Monitor.Enter(logger);
            logger.Log(logMessage);
            System.Threading.Monitor.Exit(logger);

            Dispatcher.InvokeAsync(() =>
            {
                threadStatusList.Items.Add(logMessage);
            });
        }
    }
}
