using vis1.Properties;

namespace vis1
{
    partial class About
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.version = new System.Windows.Forms.Label();
            this.autor = new System.Windows.Forms.Label();
            this.titel = new System.Windows.Forms.Label();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.version, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.autor, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.titel, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 59F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(187, 197);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // version
            // 
            this.version.Dock = System.Windows.Forms.DockStyle.Fill;
            this.version.Location = new System.Drawing.Point(3, 138);
            this.version.Name = "version";
            this.version.Size = new System.Drawing.Size(181, 59);
            this.version.TabIndex = 2;
            this.version.Text = "Version: $VERSIONSNUMMER";
            this.version.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // autor
            // 
            this.autor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.autor.Location = new System.Drawing.Point(3, 69);
            this.autor.Name = "autor";
            this.autor.Size = new System.Drawing.Size(181, 69);
            this.autor.TabIndex = 1;
            this.autor.Text = "Program Framework\r\nKillian Smith\r\n\r\nsocket support and color theme by\r\nClemens Pr" +
    "uggmayer\r\n";
            this.autor.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // titel
            // 
            this.titel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.titel.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.titel.Location = new System.Drawing.Point(3, 0);
            this.titel.Name = "titel";
            this.titel.Size = new System.Drawing.Size(181, 69);
            this.titel.TabIndex = 0;
            this.titel.Text = typeof(vis1.Properties.Resources).Name;
            this.titel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.titel.Click += new System.EventHandler(this.Titel_Click);
            // 
            // About
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(187, 197);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "About";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.About_FormClosed);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label version;
        private System.Windows.Forms.Label autor;
        private System.Windows.Forms.Label titel;
    }
}