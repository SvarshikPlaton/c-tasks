using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Packaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Path = System.IO.Path;

namespace _7z_utility
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void RunButtomClick(object sender, RoutedEventArgs e)
        {
            var sourcePath = currentFile.Text;
            var pathExtension = Path.GetExtension(sourcePath);

            var psi = new ProcessStartInfo();
            psi.FileName = "7za.exe";
            psi.RedirectStandardOutput = true;
            psi.UseShellExecute = false;

            try
            {
                if (pathExtension != ".7z")
                {
                    var archivePath = sourcePath.Replace(pathExtension, "");
                    psi.Arguments = $"a {archivePath}  {sourcePath}";
                }
                else
                {
                    var extractPath = sourcePath.Replace(Path.GetFileName(sourcePath), "");
                    psi.Arguments = $"x {sourcePath} -o{extractPath}";
                }
            }
            catch
            {
                logBox.Items.Add("Wrong file path.");
            }

            try
            {
                logBox.Items.Add("Starting process..");
                var process = Process.Start(psi);

                var reader = process.StandardOutput;
                process.WaitForExit();
                logBox.Items.Add("Process completed.");
                var outText = reader.ReadToEnd();

                if (outText.Contains("WARNING"))
                {
                    logBox.Items.Add("Error zipping file.");
                }

                var myRun = new Run(outText);
                var paragraph = new Paragraph();
                paragraph.Inlines.Add(myRun);
                var flowDoc = new FlowDocument();
                flowDoc.Blocks.Add(paragraph);
                outBox.Document = flowDoc;
            } 
            catch
            {
                logBox.Items.Add("Error when calling process 7za.exe.");
            }
        }

        private void ChooseFileButtonClick(object sender, RoutedEventArgs e)
        {
            var ofd = new OpenFileDialog();
            ofd.Filter = "7zip files (*.7z)|*.7z|All files (*.*)|*.*";
            ofd.FilterIndex = 2;

            if (ofd.ShowDialog() == true)
            {
                currentFile.Text = ofd.FileName;
            }
        }
    }
}
