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
using static System.Net.WebRequestMethods;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace UWP.Pages
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class UnitConverterPage : Page
    {
        const double LITERS_PER_GALLON = 0.264172052358148;
        const int HECTAR = 100;
        const int KILOMETER = 1000;
        const double MILE = 1609.344;
        const double YARD = 0.9144;
        public UnitConverterPage()
        {
            this.InitializeComponent();
        }

        private void ConvertLitersPer100kmToMpg(object sender, RoutedEventArgs e)
        {
            var litersPerHundredKm = double.Parse(LitersPer100kmInput.Text);

            double gallons = litersPerHundredKm * (MILE / KILOMETER) / 100;
            gallons /= LITERS_PER_GALLON;
            var mpg = 1 / gallons;
            MpgInput.Text = $"{mpg}";
        }

        private void ConvertMpgToLitersPer100km(object sender, RoutedEventArgs e)
        {
            var mpg = double.Parse(MpgInput.Text);
            double kmPerLiter = (MILE / KILOMETER) / mpg * LITERS_PER_GALLON;
            double litersPer100km = 100 / kmPerLiter;
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
                    squareMeters = sourceValue * (KILOMETER * KILOMETER);
                    break;
                case "Hectares":
                    squareMeters = sourceValue * (HECTAR * HECTAR);
                    break;
                case "Square Meters":
                    squareMeters = sourceValue;
                    break;
                default:
                    return;
            }


            double targetValue;
            switch (targetUnit)
            {
                case "Square Miles":
                    targetValue = squareMeters / (MILE * MILE);
                    break;
                case "Square Yards":
                    targetValue = squareMeters / (YARD * YARD);
                    break;
                default:
                    return;
            }

            TargetAreaInput.Text = $"{targetValue}";
        }
    }
}
