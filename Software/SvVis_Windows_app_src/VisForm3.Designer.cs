using vis1.Properties;

namespace vis1
{
  partial class VisForm3
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
            this.components = new System.ComponentModel.Container();
            this.m_Disp1 = new System.Windows.Forms.Label();
            this.m_Disp2 = new System.Windows.Forms.Label();
            this.m_Disp3 = new System.Windows.Forms.Label();
            this.m_logOutput = new System.Windows.Forms.ListBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.controMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.connectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.acqOnOffMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.emptyReceiceBufferMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.clearMessagesMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToCSVToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configurationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.colorThemeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.darkToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m_Disp4 = new System.Windows.Forms.Label();
            this.m_Disp5 = new System.Windows.Forms.Label();
            this.m_Disp6 = new System.Windows.Forms.Label();
            this.m_Disp7 = new System.Windows.Forms.Label();
            this.m_Disp8 = new System.Windows.Forms.Label();
            this.m_Disp9 = new System.Windows.Forms.Label();
            this._decodeTimer = new System.Windows.Forms.Timer(this.components);
            this.channelName9 = new System.Windows.Forms.Label();
            this.channelName8 = new System.Windows.Forms.Label();
            this.channelName7 = new System.Windows.Forms.Label();
            this.channelName6 = new System.Windows.Forms.Label();
            this.channelName5 = new System.Windows.Forms.Label();
            this.channelName4 = new System.Windows.Forms.Label();
            this.channelName3 = new System.Windows.Forms.Label();
            this.channelName2 = new System.Windows.Forms.Label();
            this.channelName1 = new System.Windows.Forms.Label();
            this.statusCheckbox = new System.Windows.Forms.CheckBox();
            this.enableChannel1 = new System.Windows.Forms.CheckBox();
            this.enableChannel2 = new System.Windows.Forms.CheckBox();
            this.enableChannel4 = new System.Windows.Forms.CheckBox();
            this.enableChannel3 = new System.Windows.Forms.CheckBox();
            this.enableChannel5 = new System.Windows.Forms.CheckBox();
            this.enableChannel6 = new System.Windows.Forms.CheckBox();
            this.enableChannel7 = new System.Windows.Forms.CheckBox();
            this.enableChannel8 = new System.Windows.Forms.CheckBox();
            this.enableChannel9 = new System.Windows.Forms.CheckBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.SetYCh3 = new System.Windows.Forms.Panel();
            this.SetY2Ch3 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch3 = new System.Windows.Forms.RadioButton();
            this.SetYCh1 = new System.Windows.Forms.Panel();
            this.SetY2Ch1 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch1 = new System.Windows.Forms.RadioButton();
            this.SetYCh2 = new System.Windows.Forms.Panel();
            this.SetY2Ch2 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch2 = new System.Windows.Forms.RadioButton();
            this.SetYCh5 = new System.Windows.Forms.Panel();
            this.SetY2Ch5 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch5 = new System.Windows.Forms.RadioButton();
            this.SetYCh4 = new System.Windows.Forms.Panel();
            this.SetY2Ch4 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch4 = new System.Windows.Forms.RadioButton();
            this.SetYCh6 = new System.Windows.Forms.Panel();
            this.SetY2Ch6 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch6 = new System.Windows.Forms.RadioButton();
            this.SetYCh7 = new System.Windows.Forms.Panel();
            this.SetY2Ch7 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch7 = new System.Windows.Forms.RadioButton();
            this.SetYCh8 = new System.Windows.Forms.Panel();
            this.SetY2Ch8 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch8 = new System.Windows.Forms.RadioButton();
            this.SetYCh9 = new System.Windows.Forms.Panel();
            this.SetY2Ch9 = new System.Windows.Forms.RadioButton();
            this.SetY1Ch9 = new System.Windows.Forms.RadioButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.pauseResumeButton = new System.Windows.Forms.Button();
            this.sendButton = new System.Windows.Forms.Button();
            this.sendBox = new System.Windows.Forms.TextBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.zedGraphLineChart = new ZedGraph.ZedGraphControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.zedGraphBarChart = new ZedGraph.ZedGraphControl();
            this.m_serialPort = new System.IO.Ports.SerialPort(this.components);
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SetYCh3.SuspendLayout();
            this.SetYCh1.SuspendLayout();
            this.SetYCh2.SuspendLayout();
            this.SetYCh5.SuspendLayout();
            this.SetYCh4.SuspendLayout();
            this.SetYCh6.SuspendLayout();
            this.SetYCh7.SuspendLayout();
            this.SetYCh8.SuspendLayout();
            this.SetYCh9.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_Disp1
            // 
            this.m_Disp1.AutoSize = true;
            this.m_Disp1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp1.Location = new System.Drawing.Point(200, 28);
            this.m_Disp1.Name = "m_Disp1";
            this.m_Disp1.Size = new System.Drawing.Size(50, 16);
            this.m_Disp1.TabIndex = 3;
            this.m_Disp1.Text = "V1111";
            // 
            // m_Disp2
            // 
            this.m_Disp2.AutoSize = true;
            this.m_Disp2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp2.Location = new System.Drawing.Point(200, 46);
            this.m_Disp2.Name = "m_Disp2";
            this.m_Disp2.Size = new System.Drawing.Size(50, 16);
            this.m_Disp2.TabIndex = 4;
            this.m_Disp2.Text = "V2222";
            // 
            // m_Disp3
            // 
            this.m_Disp3.AutoSize = true;
            this.m_Disp3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp3.Location = new System.Drawing.Point(200, 64);
            this.m_Disp3.Name = "m_Disp3";
            this.m_Disp3.Size = new System.Drawing.Size(50, 16);
            this.m_Disp3.TabIndex = 5;
            this.m_Disp3.Text = "V3333";
            // 
            // m_logOutput
            // 
            this.m_logOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_logOutput.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_logOutput.FormattingEnabled = true;
            this.m_logOutput.HorizontalScrollbar = true;
            this.m_logOutput.Location = new System.Drawing.Point(2, 39);
            this.m_logOutput.Name = "m_logOutput";
            this.m_logOutput.ScrollAlwaysVisible = true;
            this.m_logOutput.Size = new System.Drawing.Size(264, 212);
            this.m_logOutput.TabIndex = 11;
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.controMenuItem,
            this.fileToolStripMenuItem,
            this.colorThemeToolStripMenuItem,
            this.aboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(4, 2, 0, 2);
            this.menuStrip1.Size = new System.Drawing.Size(944, 24);
            this.menuStrip1.TabIndex = 12;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // controMenuItem
            // 
            this.controMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.connectToolStripMenuItem,
            this.acqOnOffMenuItem,
            this.emptyReceiceBufferMenuItem,
            this.clearMessagesMenuItem});
            this.controMenuItem.Name = "controMenuItem";
            this.controMenuItem.Size = new System.Drawing.Size(59, 20);
            this.controMenuItem.Text = "Control";
            // 
            // connectToolStripMenuItem
            // 
            this.connectToolStripMenuItem.CheckOnClick = true;
            this.connectToolStripMenuItem.Name = "connectToolStripMenuItem";
            this.connectToolStripMenuItem.Size = new System.Drawing.Size(183, 22);
            this.connectToolStripMenuItem.Text = "Connect";
            this.connectToolStripMenuItem.Click += new System.EventHandler(this.connectToolStripMenuItem_Click);
            // 
            // acqOnOffMenuItem
            // 
            this.acqOnOffMenuItem.CheckOnClick = true;
            this.acqOnOffMenuItem.Name = "acqOnOffMenuItem";
            this.acqOnOffMenuItem.Size = new System.Drawing.Size(183, 22);
            this.acqOnOffMenuItem.Text = "Acq. On/Off";
            this.acqOnOffMenuItem.Click += new System.EventHandler(this.OnAcqOnOffMenue);
            // 
            // emptyReceiceBufferMenuItem
            // 
            this.emptyReceiceBufferMenuItem.Name = "emptyReceiceBufferMenuItem";
            this.emptyReceiceBufferMenuItem.Size = new System.Drawing.Size(183, 22);
            this.emptyReceiceBufferMenuItem.Text = "Empty ReceiceBuffer";
            this.emptyReceiceBufferMenuItem.Click += new System.EventHandler(this.OnEmptyReceiveBufferMenue);
            // 
            // clearMessagesMenuItem
            // 
            this.clearMessagesMenuItem.Name = "clearMessagesMenuItem";
            this.clearMessagesMenuItem.Size = new System.Drawing.Size(183, 22);
            this.clearMessagesMenuItem.Text = "Clear Messages";
            this.clearMessagesMenuItem.Click += new System.EventHandler(this.OnClearMessagesMenue);
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToCSVToolStripMenuItem,
            this.configurationToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // saveToCSVToolStripMenuItem
            // 
            this.saveToCSVToolStripMenuItem.Enabled = false;
            this.saveToCSVToolStripMenuItem.Name = "saveToCSVToolStripMenuItem";
            this.saveToCSVToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.saveToCSVToolStripMenuItem.Text = "Save";
            this.saveToCSVToolStripMenuItem.Click += new System.EventHandler(this.saveToCSVToolStripMenuItem_Click);
            // 
            // configurationToolStripMenuItem
            // 
            this.configurationToolStripMenuItem.Name = "configurationToolStripMenuItem";
            this.configurationToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.configurationToolStripMenuItem.Text = "Configuration";
            this.configurationToolStripMenuItem.Click += new System.EventHandler(this.ConfigurationToolStripMenuItem_Click);
            // 
            // colorThemeToolStripMenuItem
            // 
            this.colorThemeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lightToolStripMenuItem,
            this.darkToolStripMenuItem});
            this.colorThemeToolStripMenuItem.Name = "colorThemeToolStripMenuItem";
            this.colorThemeToolStripMenuItem.Size = new System.Drawing.Size(87, 20);
            this.colorThemeToolStripMenuItem.Text = "Color Theme";
            // 
            // lightToolStripMenuItem
            // 
            this.lightToolStripMenuItem.Name = "lightToolStripMenuItem";
            this.lightToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.lightToolStripMenuItem.Text = "Light";
            this.lightToolStripMenuItem.Click += new System.EventHandler(this.lightToolStripMenuItem_Click);
            // 
            // darkToolStripMenuItem
            // 
            this.darkToolStripMenuItem.Name = "darkToolStripMenuItem";
            this.darkToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.darkToolStripMenuItem.Text = "Dark";
            this.darkToolStripMenuItem.Click += new System.EventHandler(this.darkToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.CheckOnClick = true;
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.AboutToolStripMenuItem_Click_1);
            // 
            // m_Disp4
            // 
            this.m_Disp4.AutoSize = true;
            this.m_Disp4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp4.Location = new System.Drawing.Point(200, 83);
            this.m_Disp4.Name = "m_Disp4";
            this.m_Disp4.Size = new System.Drawing.Size(50, 16);
            this.m_Disp4.TabIndex = 16;
            this.m_Disp4.Text = "V4444";
            // 
            // m_Disp5
            // 
            this.m_Disp5.AutoSize = true;
            this.m_Disp5.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp5.Location = new System.Drawing.Point(200, 102);
            this.m_Disp5.Name = "m_Disp5";
            this.m_Disp5.Size = new System.Drawing.Size(50, 16);
            this.m_Disp5.TabIndex = 17;
            this.m_Disp5.Text = "V5555";
            // 
            // m_Disp6
            // 
            this.m_Disp6.AutoSize = true;
            this.m_Disp6.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp6.Location = new System.Drawing.Point(200, 121);
            this.m_Disp6.Name = "m_Disp6";
            this.m_Disp6.Size = new System.Drawing.Size(50, 16);
            this.m_Disp6.TabIndex = 18;
            this.m_Disp6.Text = "V6666";
            // 
            // m_Disp7
            // 
            this.m_Disp7.AutoSize = true;
            this.m_Disp7.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp7.Location = new System.Drawing.Point(200, 139);
            this.m_Disp7.Name = "m_Disp7";
            this.m_Disp7.Size = new System.Drawing.Size(50, 16);
            this.m_Disp7.TabIndex = 20;
            this.m_Disp7.Text = "V7777";
            // 
            // m_Disp8
            // 
            this.m_Disp8.AutoSize = true;
            this.m_Disp8.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp8.Location = new System.Drawing.Point(200, 158);
            this.m_Disp8.Name = "m_Disp8";
            this.m_Disp8.Size = new System.Drawing.Size(50, 16);
            this.m_Disp8.TabIndex = 21;
            this.m_Disp8.Text = "V8888";
            // 
            // m_Disp9
            // 
            this.m_Disp9.AutoSize = true;
            this.m_Disp9.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_Disp9.Location = new System.Drawing.Point(200, 177);
            this.m_Disp9.Name = "m_Disp9";
            this.m_Disp9.Size = new System.Drawing.Size(50, 16);
            this.m_Disp9.TabIndex = 22;
            this.m_Disp9.Text = "V9999";
            // 
            // _decodeTimer
            // 
            this._decodeTimer.Tick += new System.EventHandler(this.OnDecodeTimer);
            // 
            // channelName9
            // 
            this.channelName9.AutoSize = true;
            this.channelName9.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName9.Location = new System.Drawing.Point(107, 177);
            this.channelName9.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName9.Name = "channelName9";
            this.channelName9.Size = new System.Drawing.Size(37, 16);
            this.channelName9.TabIndex = 41;
            this.channelName9.Tag = "NoTheme";
            this.channelName9.Text = "CH9";
            // 
            // channelName8
            // 
            this.channelName8.AutoSize = true;
            this.channelName8.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName8.Location = new System.Drawing.Point(107, 158);
            this.channelName8.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName8.Name = "channelName8";
            this.channelName8.Size = new System.Drawing.Size(37, 16);
            this.channelName8.TabIndex = 40;
            this.channelName8.Tag = "NoTheme";
            this.channelName8.Text = "CH8";
            // 
            // channelName7
            // 
            this.channelName7.AutoSize = true;
            this.channelName7.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName7.Location = new System.Drawing.Point(107, 139);
            this.channelName7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName7.Name = "channelName7";
            this.channelName7.Size = new System.Drawing.Size(37, 16);
            this.channelName7.TabIndex = 39;
            this.channelName7.Tag = "NoTheme";
            this.channelName7.Text = "CH7";
            // 
            // channelName6
            // 
            this.channelName6.AutoSize = true;
            this.channelName6.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName6.Location = new System.Drawing.Point(107, 121);
            this.channelName6.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName6.Name = "channelName6";
            this.channelName6.Size = new System.Drawing.Size(37, 16);
            this.channelName6.TabIndex = 38;
            this.channelName6.Tag = "NoTheme";
            this.channelName6.Text = "CH6";
            // 
            // channelName5
            // 
            this.channelName5.AutoSize = true;
            this.channelName5.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName5.Location = new System.Drawing.Point(107, 102);
            this.channelName5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName5.Name = "channelName5";
            this.channelName5.Size = new System.Drawing.Size(37, 16);
            this.channelName5.TabIndex = 37;
            this.channelName5.Tag = "NoTheme";
            this.channelName5.Text = "CH5";
            // 
            // channelName4
            // 
            this.channelName4.AutoSize = true;
            this.channelName4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName4.Location = new System.Drawing.Point(107, 83);
            this.channelName4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName4.Name = "channelName4";
            this.channelName4.Size = new System.Drawing.Size(37, 16);
            this.channelName4.TabIndex = 36;
            this.channelName4.Tag = "NoTheme";
            this.channelName4.Text = "CH4";
            // 
            // channelName3
            // 
            this.channelName3.AutoSize = true;
            this.channelName3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName3.Location = new System.Drawing.Point(107, 64);
            this.channelName3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName3.Name = "channelName3";
            this.channelName3.Size = new System.Drawing.Size(37, 16);
            this.channelName3.TabIndex = 35;
            this.channelName3.Tag = "NoTheme";
            this.channelName3.Text = "CH3";
            // 
            // channelName2
            // 
            this.channelName2.AutoSize = true;
            this.channelName2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName2.Location = new System.Drawing.Point(107, 46);
            this.channelName2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName2.Name = "channelName2";
            this.channelName2.Size = new System.Drawing.Size(37, 16);
            this.channelName2.TabIndex = 34;
            this.channelName2.Tag = "NoTheme";
            this.channelName2.Text = "CH2";
            // 
            // channelName1
            // 
            this.channelName1.AutoSize = true;
            this.channelName1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold);
            this.channelName1.Location = new System.Drawing.Point(107, 28);
            this.channelName1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.channelName1.Name = "channelName1";
            this.channelName1.Size = new System.Drawing.Size(37, 16);
            this.channelName1.TabIndex = 33;
            this.channelName1.Tag = "NoTheme";
            this.channelName1.Text = "CH1";
            // 
            // statusCheckbox
            // 
            this.statusCheckbox.AutoCheck = false;
            this.statusCheckbox.AutoSize = true;
            this.statusCheckbox.Location = new System.Drawing.Point(4, 6);
            this.statusCheckbox.Margin = new System.Windows.Forms.Padding(2);
            this.statusCheckbox.Name = "statusCheckbox";
            this.statusCheckbox.Size = new System.Drawing.Size(78, 17);
            this.statusCheckbox.TabIndex = 44;
            this.statusCheckbox.Text = "Connected";
            this.statusCheckbox.UseVisualStyleBackColor = true;
            // 
            // enableChannel1
            // 
            this.enableChannel1.AutoSize = true;
            this.enableChannel1.BackColor = System.Drawing.SystemColors.Control;
            this.enableChannel1.Checked = true;
            this.enableChannel1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel1.Location = new System.Drawing.Point(2, 30);
            this.enableChannel1.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel1.Name = "enableChannel1";
            this.enableChannel1.Size = new System.Drawing.Size(15, 14);
            this.enableChannel1.TabIndex = 45;
            this.enableChannel1.UseVisualStyleBackColor = false;
            this.enableChannel1.CheckedChanged += new System.EventHandler(this.EnableChannel1_CheckedChanged);
            // 
            // enableChannel2
            // 
            this.enableChannel2.AutoSize = true;
            this.enableChannel2.Checked = true;
            this.enableChannel2.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel2.Location = new System.Drawing.Point(2, 48);
            this.enableChannel2.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel2.Name = "enableChannel2";
            this.enableChannel2.Size = new System.Drawing.Size(15, 14);
            this.enableChannel2.TabIndex = 46;
            this.enableChannel2.UseVisualStyleBackColor = true;
            this.enableChannel2.CheckedChanged += new System.EventHandler(this.EnableChannel2_CheckedChanged);
            // 
            // enableChannel4
            // 
            this.enableChannel4.AutoSize = true;
            this.enableChannel4.Checked = true;
            this.enableChannel4.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel4.Location = new System.Drawing.Point(2, 85);
            this.enableChannel4.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel4.Name = "enableChannel4";
            this.enableChannel4.Size = new System.Drawing.Size(15, 14);
            this.enableChannel4.TabIndex = 48;
            this.enableChannel4.UseVisualStyleBackColor = true;
            this.enableChannel4.CheckedChanged += new System.EventHandler(this.EnableChannel4_CheckedChanged);
            // 
            // enableChannel3
            // 
            this.enableChannel3.AutoSize = true;
            this.enableChannel3.Checked = true;
            this.enableChannel3.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel3.Location = new System.Drawing.Point(2, 66);
            this.enableChannel3.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel3.Name = "enableChannel3";
            this.enableChannel3.Size = new System.Drawing.Size(15, 14);
            this.enableChannel3.TabIndex = 47;
            this.enableChannel3.UseVisualStyleBackColor = true;
            this.enableChannel3.CheckedChanged += new System.EventHandler(this.EnableChannel3_CheckedChanged);
            // 
            // enableChannel5
            // 
            this.enableChannel5.AutoSize = true;
            this.enableChannel5.Checked = true;
            this.enableChannel5.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel5.Location = new System.Drawing.Point(2, 104);
            this.enableChannel5.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel5.Name = "enableChannel5";
            this.enableChannel5.Size = new System.Drawing.Size(15, 14);
            this.enableChannel5.TabIndex = 49;
            this.enableChannel5.UseVisualStyleBackColor = true;
            this.enableChannel5.CheckedChanged += new System.EventHandler(this.EnableChannel5_CheckedChanged);
            // 
            // enableChannel6
            // 
            this.enableChannel6.AutoSize = true;
            this.enableChannel6.Checked = true;
            this.enableChannel6.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel6.Location = new System.Drawing.Point(2, 123);
            this.enableChannel6.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel6.Name = "enableChannel6";
            this.enableChannel6.Size = new System.Drawing.Size(15, 14);
            this.enableChannel6.TabIndex = 50;
            this.enableChannel6.UseVisualStyleBackColor = true;
            this.enableChannel6.CheckedChanged += new System.EventHandler(this.EnableChannel6_CheckedChanged);
            // 
            // enableChannel7
            // 
            this.enableChannel7.AutoSize = true;
            this.enableChannel7.Checked = true;
            this.enableChannel7.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel7.Location = new System.Drawing.Point(2, 141);
            this.enableChannel7.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel7.Name = "enableChannel7";
            this.enableChannel7.Size = new System.Drawing.Size(15, 14);
            this.enableChannel7.TabIndex = 51;
            this.enableChannel7.UseVisualStyleBackColor = true;
            this.enableChannel7.CheckedChanged += new System.EventHandler(this.EnableChannel7_CheckedChanged);
            // 
            // enableChannel8
            // 
            this.enableChannel8.AutoSize = true;
            this.enableChannel8.Checked = true;
            this.enableChannel8.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel8.Location = new System.Drawing.Point(2, 160);
            this.enableChannel8.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel8.Name = "enableChannel8";
            this.enableChannel8.Size = new System.Drawing.Size(15, 14);
            this.enableChannel8.TabIndex = 52;
            this.enableChannel8.UseVisualStyleBackColor = true;
            this.enableChannel8.CheckedChanged += new System.EventHandler(this.EnableChannel8_CheckedChanged);
            // 
            // enableChannel9
            // 
            this.enableChannel9.AutoSize = true;
            this.enableChannel9.Checked = true;
            this.enableChannel9.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableChannel9.Location = new System.Drawing.Point(2, 179);
            this.enableChannel9.Margin = new System.Windows.Forms.Padding(2);
            this.enableChannel9.Name = "enableChannel9";
            this.enableChannel9.Size = new System.Drawing.Size(15, 14);
            this.enableChannel9.TabIndex = 53;
            this.enableChannel9.UseVisualStyleBackColor = true;
            this.enableChannel9.CheckedChanged += new System.EventHandler(this.EnableChannel9_CheckedChanged);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.Location = new System.Drawing.Point(9, 25);
            this.splitContainer1.Margin = new System.Windows.Forms.Padding(2);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tabControl1);
            this.splitContainer1.Size = new System.Drawing.Size(926, 514);
            this.splitContainer1.SplitterDistance = 269;
            this.splitContainer1.SplitterWidth = 3;
            this.splitContainer1.TabIndex = 54;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Margin = new System.Windows.Forms.Padding(2);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.tableLayoutPanel1);
            this.splitContainer2.Panel1.Controls.Add(this.pauseResumeButton);
            this.splitContainer2.Panel1.Controls.Add(this.statusCheckbox);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.sendButton);
            this.splitContainer2.Panel2.Controls.Add(this.sendBox);
            this.splitContainer2.Panel2.Controls.Add(this.m_logOutput);
            this.splitContainer2.Size = new System.Drawing.Size(269, 514);
            this.splitContainer2.SplitterDistance = 235;
            this.splitContainer2.SplitterWidth = 3;
            this.splitContainer2.TabIndex = 0;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 4;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 45F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 92F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 64F));
            this.tableLayoutPanel1.Controls.Add(this.SetYCh3, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel1, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp9, 3, 9);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel9, 0, 9);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp2, 3, 2);
            this.tableLayoutPanel1.Controls.Add(this.channelName9, 2, 9);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp8, 3, 8);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel8, 0, 8);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp3, 3, 3);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp7, 3, 7);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel7, 0, 7);
            this.tableLayoutPanel1.Controls.Add(this.channelName8, 2, 8);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp4, 3, 4);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp6, 3, 6);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel6, 0, 6);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp5, 3, 5);
            this.tableLayoutPanel1.Controls.Add(this.m_Disp1, 3, 1);
            this.tableLayoutPanel1.Controls.Add(this.channelName7, 2, 7);
            this.tableLayoutPanel1.Controls.Add(this.channelName1, 2, 1);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel2, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel5, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.channelName2, 2, 2);
            this.tableLayoutPanel1.Controls.Add(this.channelName6, 2, 6);
            this.tableLayoutPanel1.Controls.Add(this.channelName3, 2, 3);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel3, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.enableChannel4, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.channelName5, 2, 5);
            this.tableLayoutPanel1.Controls.Add(this.channelName4, 2, 4);
            this.tableLayoutPanel1.Controls.Add(this.SetYCh1, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.SetYCh2, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.SetYCh5, 1, 5);
            this.tableLayoutPanel1.Controls.Add(this.SetYCh4, 1, 4);
            this.tableLayoutPanel1.Controls.Add(this.SetYCh6, 1, 6);
            this.tableLayoutPanel1.Controls.Add(this.SetYCh7, 1, 7);
            this.tableLayoutPanel1.Controls.Add(this.SetYCh8, 1, 8);
            this.tableLayoutPanel1.Controls.Add(this.SetYCh9, 1, 9);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel3, 2, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel4, 3, 0);
            this.tableLayoutPanel1.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.FixedSize;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 27);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 10;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(261, 199);
            this.tableLayoutPanel1.TabIndex = 54;
            // 
            // SetYCh3
            // 
            this.SetYCh3.Controls.Add(this.SetY2Ch3);
            this.SetYCh3.Controls.Add(this.SetY1Ch3);
            this.SetYCh3.Location = new System.Drawing.Point(47, 66);
            this.SetYCh3.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh3.Name = "SetYCh3";
            this.SetYCh3.Size = new System.Drawing.Size(56, 15);
            this.SetYCh3.TabIndex = 60;
            // 
            // SetY2Ch3
            // 
            this.SetY2Ch3.AutoSize = true;
            this.SetY2Ch3.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch3.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch3.Name = "SetY2Ch3";
            this.SetY2Ch3.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch3.TabIndex = 57;
            this.SetY2Ch3.TabStop = true;
            this.SetY2Ch3.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch3
            // 
            this.SetY1Ch3.AutoSize = true;
            this.SetY1Ch3.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch3.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch3.Name = "SetY1Ch3";
            this.SetY1Ch3.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch3.TabIndex = 0;
            this.SetY1Ch3.TabStop = true;
            this.SetY1Ch3.UseVisualStyleBackColor = true;
            this.SetY1Ch3.CheckedChanged += new System.EventHandler(this.SetY1Ch3_CheckedChanged);
            // 
            // SetYCh1
            // 
            this.SetYCh1.Controls.Add(this.SetY2Ch1);
            this.SetYCh1.Controls.Add(this.SetY1Ch1);
            this.SetYCh1.Location = new System.Drawing.Point(47, 30);
            this.SetYCh1.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh1.Name = "SetYCh1";
            this.SetYCh1.Size = new System.Drawing.Size(56, 14);
            this.SetYCh1.TabIndex = 58;
            // 
            // SetY2Ch1
            // 
            this.SetY2Ch1.AutoSize = true;
            this.SetY2Ch1.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch1.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch1.Name = "SetY2Ch1";
            this.SetY2Ch1.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch1.TabIndex = 57;
            this.SetY2Ch1.TabStop = true;
            this.SetY2Ch1.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch1
            // 
            this.SetY1Ch1.AutoSize = true;
            this.SetY1Ch1.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch1.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch1.Name = "SetY1Ch1";
            this.SetY1Ch1.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch1.TabIndex = 0;
            this.SetY1Ch1.TabStop = true;
            this.SetY1Ch1.UseVisualStyleBackColor = true;
            this.SetY1Ch1.CheckedChanged += new System.EventHandler(this.SetY1Ch1_CheckedChanged);
            // 
            // SetYCh2
            // 
            this.SetYCh2.Controls.Add(this.SetY2Ch2);
            this.SetYCh2.Controls.Add(this.SetY1Ch2);
            this.SetYCh2.Location = new System.Drawing.Point(47, 48);
            this.SetYCh2.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh2.Name = "SetYCh2";
            this.SetYCh2.Size = new System.Drawing.Size(56, 14);
            this.SetYCh2.TabIndex = 59;
            // 
            // SetY2Ch2
            // 
            this.SetY2Ch2.AutoSize = true;
            this.SetY2Ch2.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch2.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch2.Name = "SetY2Ch2";
            this.SetY2Ch2.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch2.TabIndex = 57;
            this.SetY2Ch2.TabStop = true;
            this.SetY2Ch2.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch2
            // 
            this.SetY1Ch2.AutoSize = true;
            this.SetY1Ch2.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch2.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch2.Name = "SetY1Ch2";
            this.SetY1Ch2.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch2.TabIndex = 0;
            this.SetY1Ch2.TabStop = true;
            this.SetY1Ch2.UseVisualStyleBackColor = true;
            this.SetY1Ch2.CheckedChanged += new System.EventHandler(this.SetY1Ch2_CheckedChanged);
            // 
            // SetYCh5
            // 
            this.SetYCh5.Controls.Add(this.SetY2Ch5);
            this.SetYCh5.Controls.Add(this.SetY1Ch5);
            this.SetYCh5.Location = new System.Drawing.Point(47, 104);
            this.SetYCh5.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh5.Name = "SetYCh5";
            this.SetYCh5.Size = new System.Drawing.Size(56, 15);
            this.SetYCh5.TabIndex = 62;
            // 
            // SetY2Ch5
            // 
            this.SetY2Ch5.AutoSize = true;
            this.SetY2Ch5.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch5.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch5.Name = "SetY2Ch5";
            this.SetY2Ch5.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch5.TabIndex = 57;
            this.SetY2Ch5.TabStop = true;
            this.SetY2Ch5.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch5
            // 
            this.SetY1Ch5.AutoSize = true;
            this.SetY1Ch5.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch5.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch5.Name = "SetY1Ch5";
            this.SetY1Ch5.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch5.TabIndex = 0;
            this.SetY1Ch5.TabStop = true;
            this.SetY1Ch5.UseVisualStyleBackColor = true;
            this.SetY1Ch5.CheckedChanged += new System.EventHandler(this.SetY1Ch5_CheckedChanged);
            // 
            // SetYCh4
            // 
            this.SetYCh4.Controls.Add(this.SetY2Ch4);
            this.SetYCh4.Controls.Add(this.SetY1Ch4);
            this.SetYCh4.Location = new System.Drawing.Point(47, 85);
            this.SetYCh4.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh4.Name = "SetYCh4";
            this.SetYCh4.Size = new System.Drawing.Size(56, 15);
            this.SetYCh4.TabIndex = 61;
            // 
            // SetY2Ch4
            // 
            this.SetY2Ch4.AutoSize = true;
            this.SetY2Ch4.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch4.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch4.Name = "SetY2Ch4";
            this.SetY2Ch4.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch4.TabIndex = 57;
            this.SetY2Ch4.TabStop = true;
            this.SetY2Ch4.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch4
            // 
            this.SetY1Ch4.AutoSize = true;
            this.SetY1Ch4.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch4.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch4.Name = "SetY1Ch4";
            this.SetY1Ch4.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch4.TabIndex = 0;
            this.SetY1Ch4.TabStop = true;
            this.SetY1Ch4.UseVisualStyleBackColor = true;
            this.SetY1Ch4.CheckedChanged += new System.EventHandler(this.SetY1Ch4_CheckedChanged);
            // 
            // SetYCh6
            // 
            this.SetYCh6.Controls.Add(this.SetY2Ch6);
            this.SetYCh6.Controls.Add(this.SetY1Ch6);
            this.SetYCh6.Location = new System.Drawing.Point(47, 123);
            this.SetYCh6.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh6.Name = "SetYCh6";
            this.SetYCh6.Size = new System.Drawing.Size(56, 14);
            this.SetYCh6.TabIndex = 63;
            // 
            // SetY2Ch6
            // 
            this.SetY2Ch6.AutoSize = true;
            this.SetY2Ch6.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch6.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch6.Name = "SetY2Ch6";
            this.SetY2Ch6.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch6.TabIndex = 57;
            this.SetY2Ch6.TabStop = true;
            this.SetY2Ch6.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch6
            // 
            this.SetY1Ch6.AutoSize = true;
            this.SetY1Ch6.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch6.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch6.Name = "SetY1Ch6";
            this.SetY1Ch6.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch6.TabIndex = 0;
            this.SetY1Ch6.TabStop = true;
            this.SetY1Ch6.UseVisualStyleBackColor = true;
            this.SetY1Ch6.CheckedChanged += new System.EventHandler(this.SetY1Ch6_CheckedChanged);
            // 
            // SetYCh7
            // 
            this.SetYCh7.Controls.Add(this.SetY2Ch7);
            this.SetYCh7.Controls.Add(this.SetY1Ch7);
            this.SetYCh7.Location = new System.Drawing.Point(47, 141);
            this.SetYCh7.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh7.Name = "SetYCh7";
            this.SetYCh7.Size = new System.Drawing.Size(56, 15);
            this.SetYCh7.TabIndex = 64;
            // 
            // SetY2Ch7
            // 
            this.SetY2Ch7.AutoSize = true;
            this.SetY2Ch7.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch7.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch7.Name = "SetY2Ch7";
            this.SetY2Ch7.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch7.TabIndex = 57;
            this.SetY2Ch7.TabStop = true;
            this.SetY2Ch7.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch7
            // 
            this.SetY1Ch7.AutoSize = true;
            this.SetY1Ch7.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch7.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch7.Name = "SetY1Ch7";
            this.SetY1Ch7.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch7.TabIndex = 0;
            this.SetY1Ch7.TabStop = true;
            this.SetY1Ch7.UseVisualStyleBackColor = true;
            this.SetY1Ch7.CheckedChanged += new System.EventHandler(this.SetY1Ch7_CheckedChanged);
            // 
            // SetYCh8
            // 
            this.SetYCh8.Controls.Add(this.SetY2Ch8);
            this.SetYCh8.Controls.Add(this.SetY1Ch8);
            this.SetYCh8.Location = new System.Drawing.Point(47, 160);
            this.SetYCh8.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh8.Name = "SetYCh8";
            this.SetYCh8.Size = new System.Drawing.Size(56, 15);
            this.SetYCh8.TabIndex = 65;
            // 
            // SetY2Ch8
            // 
            this.SetY2Ch8.AutoSize = true;
            this.SetY2Ch8.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch8.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch8.Name = "SetY2Ch8";
            this.SetY2Ch8.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch8.TabIndex = 57;
            this.SetY2Ch8.TabStop = true;
            this.SetY2Ch8.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch8
            // 
            this.SetY1Ch8.AutoSize = true;
            this.SetY1Ch8.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch8.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch8.Name = "SetY1Ch8";
            this.SetY1Ch8.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch8.TabIndex = 0;
            this.SetY1Ch8.TabStop = true;
            this.SetY1Ch8.UseVisualStyleBackColor = true;
            this.SetY1Ch8.CheckedChanged += new System.EventHandler(this.SetY1Ch8_CheckedChanged);
            // 
            // SetYCh9
            // 
            this.SetYCh9.Controls.Add(this.SetY2Ch9);
            this.SetYCh9.Controls.Add(this.SetY1Ch9);
            this.SetYCh9.Location = new System.Drawing.Point(47, 179);
            this.SetYCh9.Margin = new System.Windows.Forms.Padding(2);
            this.SetYCh9.Name = "SetYCh9";
            this.SetYCh9.Size = new System.Drawing.Size(56, 15);
            this.SetYCh9.TabIndex = 66;
            // 
            // SetY2Ch9
            // 
            this.SetY2Ch9.AutoSize = true;
            this.SetY2Ch9.Location = new System.Drawing.Point(37, 1);
            this.SetY2Ch9.Margin = new System.Windows.Forms.Padding(2);
            this.SetY2Ch9.Name = "SetY2Ch9";
            this.SetY2Ch9.Size = new System.Drawing.Size(14, 13);
            this.SetY2Ch9.TabIndex = 57;
            this.SetY2Ch9.TabStop = true;
            this.SetY2Ch9.UseVisualStyleBackColor = true;
            // 
            // SetY1Ch9
            // 
            this.SetY1Ch9.AutoSize = true;
            this.SetY1Ch9.Location = new System.Drawing.Point(2, 1);
            this.SetY1Ch9.Margin = new System.Windows.Forms.Padding(2);
            this.SetY1Ch9.Name = "SetY1Ch9";
            this.SetY1Ch9.Size = new System.Drawing.Size(14, 13);
            this.SetY1Ch9.TabIndex = 0;
            this.SetY1Ch9.TabStop = true;
            this.SetY1Ch9.UseVisualStyleBackColor = true;
            this.SetY1Ch9.CheckedChanged += new System.EventHandler(this.SetY1Ch9_CheckedChanged);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label2);
            this.panel1.Location = new System.Drawing.Point(47, 2);
            this.panel1.Margin = new System.Windows.Forms.Padding(2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(56, 24);
            this.panel1.TabIndex = 67;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(2, 0);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 17);
            this.label2.TabIndex = 0;
            this.label2.Text = "Y1    Y2";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.label1);
            this.panel2.Location = new System.Drawing.Point(2, 2);
            this.panel2.Margin = new System.Windows.Forms.Padding(2);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(40, 24);
            this.panel2.TabIndex = 68;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(1, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(42, 17);
            this.label1.TabIndex = 55;
            this.label1.Text = "Show";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.label3);
            this.panel3.Location = new System.Drawing.Point(107, 2);
            this.panel3.Margin = new System.Windows.Forms.Padding(2);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(70, 24);
            this.panel3.TabIndex = 69;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(2, 0);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(60, 17);
            this.label3.TabIndex = 0;
            this.label3.Text = "Channel";
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.label4);
            this.panel4.Location = new System.Drawing.Point(199, 2);
            this.panel4.Margin = new System.Windows.Forms.Padding(2);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(59, 24);
            this.panel4.TabIndex = 70;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(2, 0);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(51, 17);
            this.label4.TabIndex = 0;
            this.label4.Text = "Values";
            // 
            // pauseResumeButton
            // 
            this.pauseResumeButton.Enabled = false;
            this.pauseResumeButton.Location = new System.Drawing.Point(82, 4);
            this.pauseResumeButton.Margin = new System.Windows.Forms.Padding(2);
            this.pauseResumeButton.Name = "pauseResumeButton";
            this.pauseResumeButton.Size = new System.Drawing.Size(56, 19);
            this.pauseResumeButton.TabIndex = 55;
            this.pauseResumeButton.Text = "Pause";
            this.pauseResumeButton.UseVisualStyleBackColor = true;
            this.pauseResumeButton.Click += new System.EventHandler(this.PauseResumeButton_Click);
            // 
            // sendButton
            // 
            this.sendButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.sendButton.Enabled = false;
            this.sendButton.Location = new System.Drawing.Point(150, 4);
            this.sendButton.Name = "sendButton";
            this.sendButton.Size = new System.Drawing.Size(70, 23);
            this.sendButton.TabIndex = 13;
            this.sendButton.Text = "Send";
            this.sendButton.UseVisualStyleBackColor = true;
            this.sendButton.Click += new System.EventHandler(this.SendButton_Click);
            // 
            // sendBox
            // 
            this.sendBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.sendBox.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.sendBox.Location = new System.Drawing.Point(2, 6);
            this.sendBox.Name = "sendBox";
            this.sendBox.Size = new System.Drawing.Size(143, 20);
            this.sendBox.TabIndex = 12;
            this.sendBox.KeyUp += new System.Windows.Forms.KeyEventHandler(this.sendBox_KeyUp);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(2, 2);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(2);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(661, 512);
            this.tabControl1.TabIndex = 55;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.zedGraphLineChart);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(2);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(2);
            this.tabPage1.Size = new System.Drawing.Size(653, 486);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // zedGraphLineChart
            // 
            this.zedGraphLineChart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.zedGraphLineChart.Cursor = System.Windows.Forms.Cursors.Default;
            this.zedGraphLineChart.Location = new System.Drawing.Point(0, 0);
            this.zedGraphLineChart.Margin = new System.Windows.Forms.Padding(2);
            this.zedGraphLineChart.Name = "zedGraphLineChart";
            this.zedGraphLineChart.ScrollGrace = 0D;
            this.zedGraphLineChart.ScrollMaxX = 0D;
            this.zedGraphLineChart.ScrollMaxY = 0D;
            this.zedGraphLineChart.ScrollMaxY2 = 0D;
            this.zedGraphLineChart.ScrollMinX = 0D;
            this.zedGraphLineChart.ScrollMinY = 0D;
            this.zedGraphLineChart.ScrollMinY2 = 0D;
            this.zedGraphLineChart.Size = new System.Drawing.Size(645, 487);
            this.zedGraphLineChart.TabIndex = 43;
            this.zedGraphLineChart.UseExtendedPrintDialog = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.zedGraphBarChart);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(2);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(2);
            this.tabPage2.Size = new System.Drawing.Size(653, 486);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // zedGraphBarChart
            // 
            this.zedGraphBarChart.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.zedGraphBarChart.Location = new System.Drawing.Point(0, 0);
            this.zedGraphBarChart.Margin = new System.Windows.Forms.Padding(2);
            this.zedGraphBarChart.Name = "zedGraphBarChart";
            this.zedGraphBarChart.ScrollGrace = 0D;
            this.zedGraphBarChart.ScrollMaxX = 0D;
            this.zedGraphBarChart.ScrollMaxY = 0D;
            this.zedGraphBarChart.ScrollMaxY2 = 0D;
            this.zedGraphBarChart.ScrollMinX = 0D;
            this.zedGraphBarChart.ScrollMinY = 0D;
            this.zedGraphBarChart.ScrollMinY2 = 0D;
            this.zedGraphBarChart.Size = new System.Drawing.Size(713, 488);
            this.zedGraphBarChart.TabIndex = 0;
            this.zedGraphBarChart.UseExtendedPrintDialog = true;
            // 
            // m_serialPort
            // 
            this.m_serialPort.BaudRate = 115200;
            this.m_serialPort.ReadBufferSize = 20480;
            // 
            // VisForm3
            // 
            this.AcceptButton = this.sendButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(944, 549);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "VisForm3";
            this.Text = "SvVis3  $VERSIONSNUMMER";
            this.Load += new System.EventHandler(this.VisForm3_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel1.PerformLayout();
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.SetYCh3.ResumeLayout(false);
            this.SetYCh3.PerformLayout();
            this.SetYCh1.ResumeLayout(false);
            this.SetYCh1.PerformLayout();
            this.SetYCh2.ResumeLayout(false);
            this.SetYCh2.PerformLayout();
            this.SetYCh5.ResumeLayout(false);
            this.SetYCh5.PerformLayout();
            this.SetYCh4.ResumeLayout(false);
            this.SetYCh4.PerformLayout();
            this.SetYCh6.ResumeLayout(false);
            this.SetYCh6.PerformLayout();
            this.SetYCh7.ResumeLayout(false);
            this.SetYCh7.PerformLayout();
            this.SetYCh8.ResumeLayout(false);
            this.SetYCh8.PerformLayout();
            this.SetYCh9.ResumeLayout(false);
            this.SetYCh9.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

    }

    #endregion
    private System.Windows.Forms.Label m_Disp1;
    private System.Windows.Forms.Label m_Disp2;
    private System.Windows.Forms.Label m_Disp3;
    private System.Windows.Forms.ListBox m_logOutput;
    private System.Windows.Forms.MenuStrip menuStrip1;
    private System.Windows.Forms.ToolStripMenuItem controMenuItem;
    private System.Windows.Forms.ToolStripMenuItem acqOnOffMenuItem;
    private System.Windows.Forms.ToolStripMenuItem emptyReceiceBufferMenuItem;
    private System.Windows.Forms.ToolStripMenuItem clearMessagesMenuItem;
    private System.Windows.Forms.Label m_Disp4;
    private System.Windows.Forms.Label m_Disp5;
    private System.Windows.Forms.Label m_Disp6;
    private System.Windows.Forms.Label m_Disp7;
    private System.Windows.Forms.Label m_Disp8;
    private System.Windows.Forms.Label m_Disp9;
    private System.Windows.Forms.Timer _decodeTimer;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToCSVToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem connectToolStripMenuItem;
        private System.Windows.Forms.Label channelName9;
        private System.Windows.Forms.Label channelName8;
        private System.Windows.Forms.Label channelName7;
        private System.Windows.Forms.Label channelName6;
        private System.Windows.Forms.Label channelName5;
        private System.Windows.Forms.Label channelName4;
        private System.Windows.Forms.Label channelName3;
        private System.Windows.Forms.Label channelName2;
        private System.Windows.Forms.Label channelName1;
        private System.Windows.Forms.CheckBox statusCheckbox;
        private System.Windows.Forms.CheckBox enableChannel1;
        private System.Windows.Forms.CheckBox enableChannel2;
        private System.Windows.Forms.CheckBox enableChannel4;
        private System.Windows.Forms.CheckBox enableChannel3;
        private System.Windows.Forms.CheckBox enableChannel5;
        private System.Windows.Forms.CheckBox enableChannel6;
        private System.Windows.Forms.CheckBox enableChannel7;
        private System.Windows.Forms.CheckBox enableChannel8;
        private System.Windows.Forms.CheckBox enableChannel9;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private ZedGraph.ZedGraphControl zedGraphBarChart;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton SetY2Ch1;
        private System.Windows.Forms.RadioButton SetY1Ch1;
        private System.Windows.Forms.Panel SetYCh1;
        private System.Windows.Forms.Panel SetYCh2;
        private System.Windows.Forms.RadioButton SetY2Ch2;
        private System.Windows.Forms.RadioButton SetY1Ch2;
        private System.Windows.Forms.Panel SetYCh3;
        private System.Windows.Forms.RadioButton SetY2Ch3;
        private System.Windows.Forms.RadioButton SetY1Ch3;
        private System.Windows.Forms.Panel SetYCh5;
        private System.Windows.Forms.RadioButton SetY2Ch5;
        private System.Windows.Forms.RadioButton SetY1Ch5;
        private System.Windows.Forms.Panel SetYCh4;
        private System.Windows.Forms.RadioButton SetY2Ch4;
        private System.Windows.Forms.RadioButton SetY1Ch4;
        private System.Windows.Forms.Panel SetYCh6;
        private System.Windows.Forms.RadioButton SetY2Ch6;
        private System.Windows.Forms.RadioButton SetY1Ch6;
        private System.Windows.Forms.Panel SetYCh7;
        private System.Windows.Forms.RadioButton SetY2Ch7;
        private System.Windows.Forms.RadioButton SetY1Ch7;
        private System.Windows.Forms.Panel SetYCh8;
        private System.Windows.Forms.RadioButton SetY2Ch8;
        private System.Windows.Forms.RadioButton SetY1Ch8;
        private System.Windows.Forms.Panel SetYCh9;
        private System.Windows.Forms.RadioButton SetY2Ch9;
        private System.Windows.Forms.RadioButton SetY1Ch9;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox sendBox;
        private System.Windows.Forms.Button sendButton;
        private System.Windows.Forms.Button pauseResumeButton;
        private System.IO.Ports.SerialPort m_serialPort;
        private System.Net.Sockets.TcpClient m_socket;
        private System.Windows.Forms.ToolStripMenuItem configurationToolStripMenuItem;
        private ZedGraph.ZedGraphControl zedGraphLineChart;
        private System.Windows.Forms.ToolStripMenuItem colorThemeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lightToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem darkToolStripMenuItem;
    }
}

