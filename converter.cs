using System;
using System.Drawing;
using System.IO;

class Program
{
    static void Main(string[] args)
    {
        string inFile = @"C:\Users\elias\.gemini\antigravity\brain\2d767aad-1298-4d1d-b3f8-766b1edf65cc\wallify_icon_1779777650790.png";
        string outFile = @"C:\Users\elias\Desktop\GITHUB\wallify\wallify.ico";
        using (Bitmap bmp = new Bitmap(inFile))
        {
            // Get an HICON handle
            IntPtr hIcon = bmp.GetHicon();
            using (Icon icon = Icon.FromHandle(hIcon))
            {
                using (FileStream fs = new FileStream(outFile, FileMode.Create))
                {
                    icon.Save(fs);
                }
            }
        }
        Console.WriteLine("Done converting to ICO.");
    }
}
