using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace vis1
{
    class MenuColorTable : ProfessionalColorTable
    {
        public override Color ToolStripDropDownBackground
        {
            get
            {
                return ThemeHandler.GetBg();
            }
        }

        public override Color ImageMarginGradientBegin
        {
            get
            {
                return ThemeHandler.GetBg();
            }
        }

        public override Color ImageMarginGradientMiddle
        {
            get
            {
                return ThemeHandler.GetBg();
            }
        }

        public override Color ImageMarginGradientEnd
        {
            get
            {
                return ThemeHandler.GetBg(); ;
            }
        }

        public override Color MenuBorder
        {
            get
            {
                return Color.Black;
            }
        }

        public override Color MenuItemBorder
        {
            get
            {
                return Color.Black;
            }
        }

        public override Color MenuItemSelected
        {
            get
            {
                return ThemeHandler.GetHover();
            }
        }

        public override Color MenuStripGradientBegin
        {
            get
            {
                return ThemeHandler.GetHover();
            }
        }

        public override Color MenuStripGradientEnd
        {
            get
            {
                return ThemeHandler.GetHover();
            }
        }

        public override Color MenuItemSelectedGradientBegin
        {
            get
            {
                return ThemeHandler.GetHover();
            }
        }

        public override Color MenuItemSelectedGradientEnd
        {
            get
            {
                return ThemeHandler.GetHover();
            }
        }

        public override Color MenuItemPressedGradientBegin
        {
            get
            {
                return ThemeHandler.GetHover();
            }
        }

        public override Color MenuItemPressedGradientEnd
        {
            get
            {
                return ThemeHandler.GetHover();
            }
        }

        public override Color MenuItemPressedGradientMiddle
        {
            get
            {
                return ThemeHandler.GetHover();
            }
        }
    }
}
