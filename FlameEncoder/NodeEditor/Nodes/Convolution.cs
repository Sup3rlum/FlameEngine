using System;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Controls;

using System.Drawing;
using SixLabors.ImageSharp;

using DPoint = System.Drawing.Point;
using DColor = System.Drawing.Color;
using WColor = System.Windows.Media.Color;
using WPoint = System.Windows.Point;

using FlameEncoder.NodeEditor.Nodes.Controls;


namespace FlameEncoder.NodeEditor.Nodes
{
    public class GaussianBlurNode : Node
    {
        public override string NodeData => "";

        public int Strength = 5;

        public float[,] GenerateWeights(int kernelSize)
        {
            float sigma = (float)kernelSize / (2 * MathF.PI);
            var weights = new float[kernelSize, kernelSize];

            for (int x = 0; x < kernelSize; x++)
            {
                for (int y = 0; y < kernelSize; y++)
                {
                    float ax = x - (kernelSize - 1) / 2;
                    float ay = y - (kernelSize - 1) / 2;

                    weights[x, y] = (1.0f / (2 * MathF.PI * sigma * sigma)) * MathF.Exp(-(ax * ax + ay * ay) / (2 * sigma * sigma));
                }
            }

            return weights;
        }

        public static double ClampNorm(double x)
        {
            return Math.Max(Math.Min(x, 0.99), 0.0);
        }

        public GaussianBlurNode() : base("Gaussian Blur", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Color", InputNodeSocket.Create<DColor>(this));
            AddSocket("Color", OutputNodeSocket.Create<DColor>(this, location =>
                    {
                        float pDelta = 1.0f / 1024.0f;

                        float totalR = 0;
                        float totalG = 0;
                        float totalB = 0;

                        var weights = GenerateWeights(Strength);

                        var dim = (Strength - 1) / 2;
                        for (int x = -dim; x <= dim; x++)
                        {
                            for (int y = -dim; y <= dim;y++)
                            {
                                var locX = ClampNorm(location.X + pDelta * (double)x);
                                var locY = ClampNorm(location.Y + pDelta * (double)y);

                                var sample = (DColor)Inputs["Color"].Data(new WPoint(locX, locY));

                                float weight = weights[x + dim, y + dim];
                                totalR += (float)sample.R * weight;
                                totalG += (float)sample.G * weight;
                                totalB += (float)sample.B * weight;
                            }
                        }

                        
                        return DColor.FromArgb(255, (int)totalR, (int)totalG, (int)totalB);
                    }
                ));
        }

    }
}
