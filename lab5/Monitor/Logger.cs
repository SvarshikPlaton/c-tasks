using System;
using System.IO;


namespace Monitor
{
    public class Logger
    {
        private readonly string filePath;

        public Logger(string filePath)
        {
            this.filePath = filePath;
            ClearLogFile();
        }

        private void ClearLogFile()
        {
            File.WriteAllText(filePath, string.Empty);
        }

        public void Log(string message)
        {
            File.AppendAllText(filePath, message + Environment.NewLine);
        }
    }
}
