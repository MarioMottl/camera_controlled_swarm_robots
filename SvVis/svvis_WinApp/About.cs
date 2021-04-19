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
    public partial class About : Form
    {
        public About()
        {
            InitializeComponent();
            ThemeHandler.ApplyTheme(this);
            this.version.Text = "Version: " + vis1.Properties.Resources.Versionsnummer;
        }

        private void About_FormClosed(object sender, FormClosedEventArgs e)
        {
            //aboutToolStripMenuItem.Checked = false;
            this.Hide();
            
        }

        private void Titel_Click(object sender, EventArgs e)
        {

        }
    }
}
