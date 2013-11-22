using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace RipAllTheBrrs
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                if (args.Length != 3)
                    throw new Exception("Need the ripper, an input dir, and an output dir, dicknose!");

                var ripperPath = new FileInfo(args[0]);
                var inputDir = new DirectoryInfo(args[1]);
                var outputDir = new DirectoryInfo(args[2]);

                foreach (var f in Directory.GetFiles(inputDir.FullName).Where(x => x.EndsWith(".spc")))
                {
                    Console.WriteLine(Path.GetFileName(f));
                    using (var process = Process.Start(new ProcessStartInfo(ripperPath.FullName, "\"" + f + "\" \"" + outputDir.FullName + "/\"") { UseShellExecute = false }))
                        process.WaitForExit();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("ERROR: " + e.Message);
            }
        }
    }
}
