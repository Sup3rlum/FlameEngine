using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlameEditor.Engine
{
    public class Entity
    {
        public string Name { get; set; }
        public ObservableCollection<EntityComponent> Components { get; set; }

        public Entity(string name)
        {
            Name = name;
            Components = new ObservableCollection<EntityComponent>();
        }

    }
}
