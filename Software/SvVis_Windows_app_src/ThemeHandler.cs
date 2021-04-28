using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using vis1.Properties;
using System.Windows.Forms;


namespace vis1
{
    class ThemeHandler
    {
        public static string theme = Resources.ThemeDefault;

        public static System.Drawing.Color ColorDarkBg = System.Drawing.ColorTranslator.FromHtml("#1e1e1e");
        public static System.Drawing.Color ColorDarkFg = System.Drawing.ColorTranslator.FromHtml("#d4d4b1");
        public static System.Drawing.Color ColorDarkBorder = System.Drawing.ColorTranslator.FromHtml("#e0e0e0");
        public static System.Drawing.Color ColorDarkHover = System.Drawing.ColorTranslator.FromHtml("#094771");

        public static System.Drawing.Color ColorLightBg = System.Drawing.ColorTranslator.FromHtml("#f0f0f0");
        public static System.Drawing.Color ColorLightFg = System.Drawing.ColorTranslator.FromHtml("#000000");
        public static System.Drawing.Color ColorLightBorder = System.Drawing.ColorTranslator.FromHtml("#101010");
        public static System.Drawing.Color ColorLightHover = System.Drawing.ColorTranslator.FromHtml("#e5e5e5");

        public static System.Drawing.Color GetBg()
        {
            if (theme == Resources.ThemeDark)
            {
                return ColorDarkBg;
            }
            else if (theme == Resources.ThemeLight)
            {
                return ColorLightBg;
            }
            else
            {
                theme = Resources.ThemeDefault;
                return GetBg();
            }
        }

        public static System.Drawing.Color GetFg()
        {
            if (theme == Resources.ThemeDark)
            {
                return ColorDarkFg;
            }
            else if (theme == Resources.ThemeLight)
            {
                return ColorLightFg;
            }
            else
            {
                theme = Resources.ThemeDefault;
                return GetBg();
            }
        }

        public static System.Drawing.Color GetBorder()
        {
            if (theme == Resources.ThemeDark)
            {
                return ColorDarkBorder;
            }
            else if (theme == Resources.ThemeLight)
            {
                return ColorLightBorder;
            }
            else
            {
                theme = Resources.ThemeDefault;
                return GetHover();
            }
        }

        public static System.Drawing.Color GetHover()
        {
            if (theme == Resources.ThemeDark)
            {
                return ColorDarkHover;
            }
            else if (theme == Resources.ThemeLight)
            {
                return ColorLightHover;
            }
            else
            {
                theme = Resources.ThemeDefault;
                return GetHover();
            }
        }

        public static void ApplyTheme(Control target)
        {
            // get default colors
            System.Drawing.Color ColorFg = GetFg();
            System.Drawing.Color ColorBg = GetBg();
            // recursively apply theme
            foreach (Control component in target.Controls)
            {
                ApplyTheme(component);
            }
            // skip over theme-excluded stuff
            if(!(target.Tag is null))
            {
                if(target.Tag.ToString().Contains("NoTheme"))
                {
                    return;
                }
            }
            // apply default colors
            target.BackColor = ColorBg;
            target.ForeColor = ColorFg;
            // Object Dependent Custom Colors
            if (target is TabControl)
            {
                TabControl tabComponent = (TabControl)target;
                tabComponent.DrawMode = TabDrawMode.Normal;
            }
            else if(target is CheckBox)
            {
                CheckBox box = (CheckBox)target;
            }
            else if (target is ZedGraph.ZedGraphControl)
            {
                ZedGraph.ZedGraphControl componentGraph = (ZedGraph.ZedGraphControl)target;
                ZedGraph.GraphPane pane = componentGraph.GraphPane;
                pane.Chart.Fill.Type = ZedGraph.FillType.Solid;
                pane.Chart.Fill.Color = ColorBg;
                pane.Chart.Border.Color = ColorFg;
                pane.Legend.Fill.Type = ZedGraph.FillType.Solid;
                pane.Legend.Fill.Color = ColorBg;
                pane.Legend.Border.Color = ColorFg;
                pane.Legend.FontSpec.FontColor = ColorFg;
                pane.Fill.Color = ColorBg;

                foreach(var axis in pane.YAxisList)
                {
                    axis.Color = ColorFg;
                    axis.Scale.FontSpec.FontColor = ColorFg;
                    axis.MajorTic.Color = ColorFg;
                    axis.MinorTic.Color = ColorFg;
                }
                foreach (var axis in pane.Y2AxisList)
                {
                    axis.Color = ColorFg;
                    axis.Scale.FontSpec.FontColor = ColorFg;
                    axis.MajorTic.Color = ColorFg;
                    axis.MinorTic.Color = ColorFg;
                }
                pane.XAxis.Color = ColorFg;
                pane.XAxis.Scale.FontSpec.FontColor = ColorFg;
                pane.XAxis.MajorTic.Color = ColorFg;
                pane.XAxis.MinorTic.Color = ColorFg;

                pane.YAxis.Color = ColorFg;
                pane.YAxis.Scale.FontSpec.FontColor = ColorFg;
                pane.YAxis.MajorTic.Color = ColorFg;
                pane.YAxis.MinorTic.Color = ColorFg;
            }
            target.Refresh();
        }

        public static void ApplyMenu(ToolStripItemCollection items)
        {
            foreach(ToolStripMenuItem item in items)
            {
                item.ForeColor = GetFg();
                item.BackColor = GetBg();
                ApplyMenu(item.DropDownItems);
            }
        }
    }
}
