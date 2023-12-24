using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileManager.Models
{
    internal class FileListItem
    {
        public string Name { get; set; }
        public bool IsDirectory { get; set; }
        public string Path { get; set; }
        public string Type { get; set; }
        public string DateCreatedString { get; set; }
        public string SizeString { get; set; }
    }
}
