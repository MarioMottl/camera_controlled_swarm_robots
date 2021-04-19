using System;
using System.IO.Ports;
using System.Windows.Forms;

namespace vis1
{
    internal static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static int Main()
        {
			/*
            if (!(SerialPort.GetPortNames().Length > 0))   //Starte Programm nur wenn COM-Schnitstellen verfügbar sind
            {
                MessageBox.Show("No Serial Ports available", "SVis", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                return 1;
            }
			*/
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new VisForm3());    //Begins running a standard application message loop on the current thread, and makes the specified form visible. https://msdn.microsoft.com/en-us/library/system.windows.forms.application.run(v=vs.110).aspx 2016/09/09        }
            return 0;
        }
    }
}