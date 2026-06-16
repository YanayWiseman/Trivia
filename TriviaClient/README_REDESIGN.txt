Machshilim red/yellow cyber redesign

1. Replace your existing XAML files with these files.
2. Put your MP4 at Assets/background.mp4.
3. In Visual Studio select the MP4 and set:
   Build Action: Content
   Copy to Output Directory: Copy if newer
4. The video is hosted once in MainWindow behind every Page. No page logic, control names, or click handlers were intentionally changed.

Note: MediaElement does not loop by XAML alone. It plays once. To loop it, handle MediaEnded in MainWindow.xaml.cs and reset Position to TimeSpan.Zero, then Play().
