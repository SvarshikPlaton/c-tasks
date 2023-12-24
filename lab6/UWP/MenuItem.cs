using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Controls;

namespace UWP
{
    public enum MenuItemIdentifier
    {
        Home,
        About,
        UnitConverter
    }

    public class MenuItem
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public Symbol Icon { get; set; }
        public MenuItemIdentifier Identifier { get; set; }
    }
}

