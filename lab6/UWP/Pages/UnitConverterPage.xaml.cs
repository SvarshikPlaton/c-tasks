using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace UWP.Pages
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class UnitConverterPage : Page
    {
        public UnitConverterPage()
        {
            this.InitializeComponent();
        }

        private void ConvertLitersPer100kmToMpg(object sender, RoutedEventArgs e)
        {
            var litersPerHundredKm = double.Parse(LitersPer100kmInput.Text);
            var mpg = litersPerHundredKm * 235.21;
            MpgInput.Text = $"{mpg}";
        }

        private void ConvertMpgToLitersPer100km(object sender, RoutedEventArgs e)
        {
            var mpg = double.Parse(MpgInput.Text);
            var litersPer100km = mpg / 235.21; 
            LitersPer100kmInput.Text = $"{litersPer100km}";
        }

        private void ConvertArea(object sender, RoutedEventArgs e)
        {
            var sourceValue = double.Parse(SourceAreaInput.Text);
            var sourceUnit = ((ComboBoxItem)SourceAreaUnit.SelectedItem).Content.ToString();
            var targetUnit = ((ComboBoxItem)TargetAreaUnit.SelectedItem).Content.ToString();

            double squareMeters;
            switch (sourceUnit)
            {
                case "Square Kilometers":
                    squareMeters = sourceValue * 1e6;
                    break;
                case "Hectares":
                    squareMeters = sourceValue * 1e4;
                    break;
                default:
                    squareMeters = sourceValue;
                    break;
            }


            double targetValue;
            switch (targetUnit)
            {
                case "Square Miles":
                    targetValue = squareMeters * 3.861e-7;
                    break;
                default: 
                    targetValue = squareMeters * 1.196;
                    break;
            }

            TargetAreaInput.Text = $"{targetValue}";
        }
    }
}
