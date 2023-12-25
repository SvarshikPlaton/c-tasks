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
                    const int kilometer = 1000;
                    const int squareKilometer = kilometer * kilometer;
                    squareMeters = sourceValue * squareKilometer;
                    break;
                case "Hectares":
                    const int hectar = 100;
                    const int squareHectar = hectar * hectar;
                    squareMeters = sourceValue * squareHectar;
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
                    const double mile = 1609.344;
                    const double squareMile = mile * mile;
                    targetValue = squareMeters / squareMile;
                    break;
                case "Square Yards":
                    const double yard = 0.9144;
                    const double squareYard = yard * yard;
                    targetValue = squareMeters / squareYard;
                    break;
                default:
                    return;
            }

            TargetAreaInput.Text = $"{targetValue}";
        }
    }
}
