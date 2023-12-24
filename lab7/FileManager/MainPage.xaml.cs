using FileManager.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace FileManager
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private ObservableCollection<FileListItem> fileListItems = new ObservableCollection<FileListItem>();
        public MainPage()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }

        private async void ShowErrorMessage(string text)
        {
            var textBlock = new TextBlock();
            textBlock.Text = text;

            var dialog = new ContentDialog()
            {
                Title = "An Error Occured",
                Content = textBlock,
                PrimaryButtonText = "OK"
            };
            await dialog.ShowAsync();
        }

        private async void ReloadFileListContents(StorageFolder folder)
        {
            fileListItems.Clear();

            var storageFolders = await folder.GetFoldersAsync();
            foreach (var storageFolder in storageFolders)
            {
                fileListItems.Add(new FileListItem()
                {
                    Name = storageFolder.DisplayName,
                    IsDirectory = true,
                    Type = storageFolder.DisplayType,
                    Path = storageFolder.Path,
                    SizeString = "",
                    DateCreatedString = ""
                });
            }

            var storageFiles = await folder.GetFilesAsync();
            foreach (var file in storageFiles)
            {
                var properties = await file.GetBasicPropertiesAsync();

                fileListItems.Add(new FileListItem()
                {
                    Name = file.DisplayName,
                    IsDirectory = false,
                    Type = file.DisplayType,
                    Path = file.Path,
                    SizeString = SizeToString(properties.Size),
                    DateCreatedString = file.DateCreated.ToString("hh:mm dddd, MMMM dd, yyyy")
                });
            }

            FileListView.ItemsSource = fileListItems;
            WorkingDirectoryTextBox.Text = folder.Path;
        }

        public async void PickFolderButton_Click(object sender, RoutedEventArgs e)
        {
            var folderPicker = new FolderPicker();
            folderPicker.SuggestedStartLocation = PickerLocationId.Desktop;
            folderPicker.FileTypeFilter.Add("*");

            StorageFolder folder = await folderPicker.PickSingleFolderAsync();
            if(folder == null)
            {
                return;
            }

            Windows.Storage.AccessCache.StorageApplicationPermissions.FutureAccessList.AddOrReplace("PickedFolderToken", folder);
            ReloadFileListContents(folder);
        }

        private string SizeToString(ulong size)
        {
            var selectedItem = (ComboBoxItem)SizeUnitCombobox.SelectedItem;
            var unit = selectedItem.Content.ToString();

            if (unit == "Kilobytes")
            {
                return $"{size / 1024.0} KB";
            }
            else if (unit == "Megabytes")
            {
                return $"{size / 1024.0 / 1024.0} MB";
            }

            return $"{size} B";
        }

        private async void FileListView_DoubleTapped(object sender, DoubleTappedRoutedEventArgs e)
        {
            var item = (sender as ListView).SelectedItem as FileListItem;
            if(!item.IsDirectory)
            {
                return;
            }

            try
            {
                var folder = await StorageFolder.GetFolderFromPathAsync(item.Path);
                ReloadFileListContents(folder);
            } 
            catch(Exception ex)
            {
                ShowErrorMessage(ex.Message);
            }
        }

        private async void CreateFolderButton_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new ContentDialog()
            {
                Title = "Folder Name",
                Content = new TextBox(),
                CloseButtonText = "Cancel",
                PrimaryButtonText = "Ok"
            };

            var result = await dialog.ShowAsync();
            if (result == ContentDialogResult.Primary)
            {
                TextBox textBox = (TextBox)dialog.Content;
                string newFolderName = textBox.Text;

                var parentFolder = await StorageFolder.GetFolderFromPathAsync(WorkingDirectoryTextBox.Text);
                await parentFolder.CreateFolderAsync(newFolderName, CreationCollisionOption.OpenIfExists);
                ReloadFileListContents(parentFolder);
            }
        }

        private async void DeleteFolderButton_Click(object sender, RoutedEventArgs e)
        {
            if(FileListView.SelectedIndex == -1)
            {
                return;
            }

            var folder = await StorageFolder.GetFolderFromPathAsync(fileListItems[FileListView.SelectedIndex].Path);
            await folder.DeleteAsync();

            var workingFolder = await StorageFolder.GetFolderFromPathAsync(WorkingDirectoryTextBox.Text);
            ReloadFileListContents(workingFolder);
        }

        private async void SizeUnitCombobox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(WorkingDirectoryTextBox.Text == string.Empty)
            {
                return;
            }

            var workingFolder = await StorageFolder.GetFolderFromPathAsync(WorkingDirectoryTextBox.Text);
            ReloadFileListContents(workingFolder);
        }
    }
}
