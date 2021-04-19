using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Configuration;

namespace vis1
{
    public partial class Settings : Form
    {

        public Settings()
        {
            InitializeComponent();

            SettingsElements.m_enableButtonsSettings[0] = EnableCH1;
            SettingsElements.m_enableButtonsSettings[1] = EnableCH2;
            SettingsElements.m_enableButtonsSettings[2] = EnableCH3;
            SettingsElements.m_enableButtonsSettings[3] = EnableCH4;
            SettingsElements.m_enableButtonsSettings[4] = EnableCH5;
            SettingsElements.m_enableButtonsSettings[5] = EnableCH6;
            SettingsElements.m_enableButtonsSettings[6] = EnableCH7;
            SettingsElements.m_enableButtonsSettings[7] = EnableCH8;
            SettingsElements.m_enableButtonsSettings[8] = EnableCH9;

            SettingsElements.m_selectY1Settings[0] = SetY1CH1;
            SettingsElements.m_selectY1Settings[1] = SetY1CH2;
            SettingsElements.m_selectY1Settings[2] = SetY1CH3;
            SettingsElements.m_selectY1Settings[3] = SetY1CH4;
            SettingsElements.m_selectY1Settings[4] = SetY1CH5;
            SettingsElements.m_selectY1Settings[5] = SetY1CH6;
            SettingsElements.m_selectY1Settings[6] = SetY1CH7;
            SettingsElements.m_selectY1Settings[7] = SetY1CH8;
            SettingsElements.m_selectY1Settings[8] = SetY1CH9;

            SettingsElements.m_selectY2Settings[0] = SetY2CH1;
            SettingsElements.m_selectY2Settings[1] = SetY2CH2;
            SettingsElements.m_selectY2Settings[2] = SetY2CH3;
            SettingsElements.m_selectY2Settings[3] = SetY2CH4;
            SettingsElements.m_selectY2Settings[4] = SetY2CH5;
            SettingsElements.m_selectY2Settings[5] = SetY2CH6;
            SettingsElements.m_selectY2Settings[6] = SetY2CH7;
            SettingsElements.m_selectY2Settings[7] = SetY2CH8;
            SettingsElements.m_selectY2Settings[8] = SetY2CH9;

            SettingsElements.m_channelNameLabelsSettings[0] = SetNameCH1;
            SettingsElements.m_channelNameLabelsSettings[1] = SetNameCH2;
            SettingsElements.m_channelNameLabelsSettings[2] = SetNameCH3;
            SettingsElements.m_channelNameLabelsSettings[3] = SetNameCH4;
            SettingsElements.m_channelNameLabelsSettings[4] = SetNameCH5;
            SettingsElements.m_channelNameLabelsSettings[5] = SetNameCH6;
            SettingsElements.m_channelNameLabelsSettings[6] = SetNameCH7;
            SettingsElements.m_channelNameLabelsSettings[7] = SetNameCH8;
            SettingsElements.m_channelNameLabelsSettings[8] = SetNameCH9;

            SettingsElements.m_axisValuesSettings[0] = SetMinX;
            SettingsElements.m_axisValuesSettings[1] = SetMaxX;
            SettingsElements.m_axisValuesSettings[2] = SetMinY1;
            SettingsElements.m_axisValuesSettings[3] = SetMaxY1;
            SettingsElements.m_axisValuesSettings[4] = SetMinY2;
            SettingsElements.m_axisValuesSettings[5] = SetMaxY2;

            LoadSettings();
            ThemeHandler.ApplyTheme(this);
        }
        public void LoadSettings()  //Loads initial Settings into Elements and axis
        {
            for (int i = 0; i < SettingsElements.m_enableButtonsSettings.Length; i++)
            {
                SettingsElements.m_enableButtonsSettings[i].Checked = ConfigObject.configuration.channels[i].enable;
            }

            for (int i = 0; i < SettingsElements.m_selectY1Settings.Length; i++)
            {
                SettingsElements.m_selectY1Settings[i].Checked = !ConfigObject.configuration.channels[i].IsY2;
            }

            for (int i = 0; i < SettingsElements.m_selectY2Settings.Length; i++)
            {
                SettingsElements.m_selectY2Settings[i].Checked = ConfigObject.configuration.channels[i].IsY2;
            }

            for (int i = 0; i < SettingsElements.m_channelNameLabelsSettings.Length; i++)
            {
                SettingsElements.m_channelNameLabelsSettings[i].Text = ConfigObject.configuration.channels[i].name;
            }

            SettingsElements.m_axisValuesSettings[0].Text = Convert.ToString(ConfigObject.configuration.axis[0].min);
            SettingsElements.m_axisValuesSettings[1].Text = Convert.ToString(ConfigObject.configuration.axis[0].max);

            SettingsElements.m_axisValuesSettings[2].Text = Convert.ToString(ConfigObject.configuration.axis[1].min);
            SettingsElements.m_axisValuesSettings[3].Text = Convert.ToString(ConfigObject.configuration.axis[1].max);

            SettingsElements.m_axisValuesSettings[4].Text = Convert.ToString(ConfigObject.configuration.axis[2].min);
            SettingsElements.m_axisValuesSettings[5].Text = Convert.ToString(ConfigObject.configuration.axis[2].max);
        }

        private void ButtonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ButtonApply_Click(object sender, EventArgs e)
        {
            ConfigObject.configuration.ApplySettings();
            this.Close();
        }

        private void ButtonReset_Click(object sender, EventArgs e)
        {
            ConfigObject.configuration.InitializeSettings(); 
            LoadSettings();
        }

        private void ButtonSaveToFile_Click(object sender, EventArgs e)
        {
            ConfigObject.configuration.SaveToFile();
        }
    }

}
