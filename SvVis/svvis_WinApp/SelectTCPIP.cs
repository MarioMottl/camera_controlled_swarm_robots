using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace vis1
{
    public partial class SelectTCPIP : Form
    {
        public string ipAddress;
        public Int16 port;
        public bool error = true;
        public SelectTCPIP()
        {
            InitializeComponent();
            AcceptButton = buttonOK;
            ThemeHandler.ApplyTheme(this);
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            try
            {
                this.ipAddress = this.textBoxIP.Text; // Save IP address as String
                this.port = Int16.Parse(this.textBoxPort.Text);    // Save Port Number / servive as String
                error = false; // Indicate that no error happened
                this.Close();
            }
            catch(Exception ex)
            {
                if( ex is ArgumentNullException || ex is FormatException || ex is OverflowException) // Parse failed
                {
                    this.error = true; // indicate that an error happened
                    this.Close();
                }
            }
        }
    }
}
