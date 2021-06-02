using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlameEditor.Engine
{
    public class Scene
    {
        public string Name { get; set; }
        public ObservableCollection<Entity> EntityCollection { get; set; }

        public Scene(string name)
        {
            Name = name;
            EntityCollection = new ObservableCollection<Entity>();
        }

    }
}
