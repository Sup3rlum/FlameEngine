using FlameEncoder.NodeEditor.Nodes.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

using SixLabors.ImageSharp;

using DPoint = System.Drawing.Point;
using DColor = System.Drawing.Color;
using WColor = System.Windows.Media.Color;
using System.Windows.Controls;

using FlameEncoder.NodeEditor.Nodes.Controls;
using System.Windows.Media.Media3D;
using FlameEncoder.Data;

namespace FlameEncoder.NodeEditor.Nodes
{


    public class FaceListToCubeMapNode : Node
    {

        public string[] FaceNames =
        {
            "+X",
            "-X",
            "+Y",
            "-Y",
            "+Z",
            "-Z"
        };

        public override string NodeData => "";

        public FaceListToCubeMapNode() : base("Face List To Cube Map", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("+X", InputNodeSocket.Create<Color32>(this));
            AddSocket("-X", InputNodeSocket.Create<Color32>(this));
            AddSocket("+Y", InputNodeSocket.Create<Color32>(this));
            AddSocket("-Y", InputNodeSocket.Create<Color32>(this));
            AddSocket("+Z", InputNodeSocket.Create<Color32>(this));
            AddSocket("-Z", InputNodeSocket.Create<Color32>(this));

            AddSocket("Cubemap", OutputNodeSocket.Create<Color32>(this,
                (location) =>
                {
                    var inputName = FaceNames[location.Layer];
                    var color = ((Color32)Inputs[inputName].Data(location));

                    return color;
                }));
        }
    }
    public class EquirectangularToCubeMapNode : Node
    {

        // Define our six cube faces.
        // 0 - 3 are side faces, clockwise order
        // 4 and 5 are top and bottom, respectively
        float[,] faceTransform =
        {
            { 0, 0},
            { MathF.PI, 0},
            { 0, -MathF.PI / 2},
            { 0, MathF.PI / 2},
            { -MathF.PI / 2, 0},
            { MathF.PI / 2, 0}
        };

        // Map a part of the equirectangular panorama (in) to a cube face
        // (face). The ID of the face is given by faceId. The desired
        // width and height are given by width and height.
        public (double, double) CubeMapToEquirectCoords(PixelPosition pos)
        {

            float x = (float)pos.X;
            float y = (float)pos.Y;
            int faceId = pos.Layer;


            // Calculate adjacent (ak) and opposite (an) of the
            // triangle that is spanned from the sphere center
            //to our cube face.
            float an = MathF.Sin(MathF.PI / 4);
            float ak = MathF.Cos(MathF.PI / 4);

            float ftu = faceTransform[faceId, 0];
            float ftv = faceTransform[faceId, 1];


            // Map face pixel coordinates to [-1, 1] on plane
            float nx = x - 0.5f;
            float ny = y - 0.5f;

            nx *= 2;
            ny *= 2;

            // Map [-1, 1] plane coords to [-an, an]
            // thats the coordinates in respect to a unit sphere
            // that contains our box.
            nx *= an;
            ny *= an;

            float u, v;

            // Project from plane to sphere surface.
            if (ftv == 0)
            {
                // Center faces
                u = MathF.Atan2(nx, ak);
                v = MathF.Atan2(ny * MathF.Cos(u), ak);
                u += ftu;
            }
            else if (ftv > 0)
            {
                // Bottom face
                float d = MathF.Sqrt(nx * nx + ny * ny);
                v = MathF.PI / 2 - MathF.Atan2(d, ak);
                u = MathF.Atan2(ny, nx);
            }
            else
            {
                // Top face
                float d = MathF.Sqrt(nx * nx + ny * ny);
                v = -MathF.PI / 2 + MathF.Atan2(d, ak);
                u = MathF.Atan2(-ny, nx);
            }

            // Map from angular coordinates to [-1, 1], respectively.
            u = u / (MathF.PI);
            v = v / (MathF.PI / 2);

            // Warp around, if our coordinates are out of bounds.
            while (v < -1)
            {
                v += 2;
                u += 1;
            }
            while (v > 1)
            {
                v -= 2;
                u += 1;
            }

            while (u < -1)
            {
                u += 2;
            }
            while (u > 1)
            {
                u -= 2;
            }

            // Map from [-1, 1] to in texture space
            u = u / 2.0f + 0.5f;
            v = v / 2.0f + 0.5f;

            u *= 0.9999f;
            v *= 0.9999f;

            return (u, v);
        }

        public override string NodeData => "";

        public EquirectangularToCubeMapNode() : base("Equirectagular To Cube Map", new EquirectangularToCubemapNodeControl(), Colors.Yellow)
        {
            AddSocket("Equirectangular", InputNodeSocket.Create<Color32>(this));

            AddSocket("Cubemap", OutputNodeSocket.Create<Color32>(this,
                (location) =>
                {
                    var (u, v) = CubeMapToEquirectCoords(location);
                    var color = ((Color32)Inputs["Equirectangular"].Data(new PixelPosition(u, v, 0)));

                    return color;
                }));
        }
    }
}
