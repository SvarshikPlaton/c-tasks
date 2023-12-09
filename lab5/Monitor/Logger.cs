using System;
using System.IO;


namespace Monitor
{
    public class Logger
    {
        private readonly string filePath;
        private readonly object lockObject = new object();

        public Logger(string filePath)
        {
            this.filePath = filePath;
            ClearLogFile();
        }

        private void ClearLogFile()
        {
            lock (lockObject)
            {
                File.WriteAllText(filePath, string.Empty);
            }
        }

        public void Log(string message)
        {
            lock (lockObject)
            {
                File.AppendAllText(filePath, message + Environment.NewLine);
            }
        }
    }
}
