using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Drawing;
using System.IO;



namespace GreyscaleConverter
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private string fileName;
        private static int width, height;
        private Int32Rect rect;
        private byte[] inputPixels, gs1p, gs2p, gs3p;
        private WriteableBitmap gs1bm, gs2bm, gs3bm;

        private void ChooseFileClicked(object sender, RoutedEventArgs e)
        {
            var dialog = new Microsoft.Win32.OpenFileDialog();
            dialog.Filter = "Image Files(*.jpg; *.png; *.bmp)| *.jpg; *.png; *.bmp;";

            // Show open file dialog box
            bool? result = dialog.ShowDialog();

            // Process open file dialog box results
            if (result == true)
            {
                fileName = dialog.FileName;

                // display input image
                Uri fileUri = new Uri(fileName);
                inputImage.Source = new BitmapImage(fileUri);

                // create bitmap of input image
                BitmapImage inputBitmap = new BitmapImage(fileUri);

                // get the name of input file
                fileName = System.IO.Path.GetFileName(fileName);
                fileName = System.IO.Path.ChangeExtension(fileName, null);

                // get dimensions of the image
                width = inputBitmap.PixelWidth;
                height = inputBitmap.PixelHeight;

                // arrays to store the pixels data
                inputPixels = new byte[width * height * 4];
                gs1p = new byte[width * height * 4];
                gs2p = new byte[width * height * 4];
                gs3p = new byte[width * height * 4];

                //bitmaps for converted images
                gs1bm = new WriteableBitmap(inputBitmap);
                gs2bm = new WriteableBitmap(inputBitmap);
                gs3bm = new WriteableBitmap(inputBitmap);

                //rectangle covering whole image
                rect = new Int32Rect(0, 0, width, height);

                // ,atrices for  zooming and panning the images
                Matrix m = new Matrix(1, 0, 0, 1, 0, 0);
                inputImage.RenderTransform = new MatrixTransform(m);
                grayscale1.RenderTransform = new MatrixTransform(m);
                grayscale2.RenderTransform = new MatrixTransform(m);
                grayscale3.RenderTransform = new MatrixTransform(m);

                // copy pixels from input bitmap to array
                inputBitmap.CopyPixels(inputPixels, width * 4, 0);

                // min and max values for desaturation algorithm
                byte min, max;

                // convert the image to grayscale with 3 different algorithms
                for (int i = 0; i < inputPixels.Length; i += 4)
                {
                    // AVERAGE ALGORITHM

                    gs1p[i] = (byte)((inputPixels[i] + inputPixels[i + 1] + inputPixels[i + 2]) / 3);
                    gs1p[i + 1] = gs1p[i];
                    gs1p[i + 2] = gs1p[i];
                    gs1p[i + 3] = inputPixels[i + 3];

                    // DESATURATION ALGORITHM

                    if (inputPixels[i] > inputPixels[i + 1])
                    {
                        max = inputPixels[i];
                        min = inputPixels[i + 1];
                    }
                    else
                    {
                        max = inputPixels[i + 1];
                        min = inputPixels[i];
                    }
                    if (inputPixels[i + 2] > max)
                        max = inputPixels[i + 2];
                    else if (inputPixels[i + 2] < min)
                        min = inputPixels[i + 2];
                    gs2p[i] = (byte)((max + min) / 2);
                    gs2p[i + 1] = gs2p[i];
                    gs2p[i + 2] = gs2p[i];
                    gs2p[i + 3] = inputPixels[i + 3];

                    // LUMINOSITY ALGORITHM

                    gs3p[i] = (byte)(inputPixels[i] * 0.07 + inputPixels[i + 1] * 0.72 + inputPixels[i + 2] * 0.21);
                    gs3p[i + 1] = gs3p[i];
                    gs3p[i + 2] = gs3p[i];
                    gs3p[i + 3] = inputPixels[i + 3];
                }

                // write converted pixels to bitmaps
                gs1bm.WritePixels(rect, gs1p, width * 4, 0);
                gs2bm.WritePixels(rect, gs2p, width * 4, 0);
                gs3bm.WritePixels(rect, gs3p, width * 4, 0);

                // display the results
                grayscale1.Source = gs1bm;
                grayscale2.Source = gs2bm;
                grayscale3.Source = gs3bm;

                // enable "save" buttons
                saveGS1.Visibility = Visibility.Visible;
                saveGS2.Visibility = Visibility.Visible;
                saveGS3.Visibility = Visibility.Visible;
            }
        }

        // save the image
        private void save(WriteableBitmap bitmap, string identifier)
        {
            var dialog = new Microsoft.Win32.SaveFileDialog();
            dialog.FileName = fileName + '_' + identifier; // Default file name
            dialog.DefaultExt = ".png"; // Default file extension
            dialog.Filter = "Images (.png)|*.png"; // Filter files by extension

            // Show save file dialog box
            bool? result = dialog.ShowDialog();

            // Process save file dialog box results
            if (result == true)
            {
                // Save image
                BitmapEncoder encoder = new PngBitmapEncoder();
                encoder.Frames.Add(BitmapFrame.Create(bitmap));

                using (var fileStream = new System.IO.FileStream(dialog.FileName, System.IO.FileMode.Create))
                {
                    encoder.Save(fileStream);
                }
            }
        }

        // keep the image within its bounds
        private void fitToBounds(Image image)
        {
            Matrix m = image.RenderTransform.Value;

            if (m.OffsetX > 0)
                m.OffsetX = 0;
            else if (m.OffsetX - image.ActualWidth < -image.ActualWidth * m.M11)
                m.OffsetX = image.ActualWidth * (1 - m.M11);

            if (m.OffsetY > 0)
                m.OffsetY = 0;
            else if (m.OffsetY - image.ActualHeight < -image.ActualHeight * m.M22)
                m.OffsetY = image.ActualHeight * (1 - m.M22);

            image.RenderTransform = new MatrixTransform(m);

        }

        // zoom the image
        private void zoomImage(object sender, MouseWheelEventArgs e)
        {
            if (sender is Border border)
            {
                if (border.Child is Image image)
                {
                    System.Windows.Point p = e.MouseDevice.GetPosition(image);

                    Matrix m = image.RenderTransform.Value;
                    if (e.Delta > 0)
                        m.ScaleAtPrepend(1.1, 1.1, p.X, p.Y);
                    else
                        m.ScaleAtPrepend(1 / 1.1, 1 / 1.1, p.X, p.Y);

                    if (m.M11 < 1)
                    {
                        m.M11 = 1;
                        m.M22 = 1;
                    }

                    image.RenderTransform = new MatrixTransform(m);

                    fitToBounds(image);

                    e.Handled = true;
                }

            }
        }

        private System.Windows.Point grabbingPoint;
        private System.Windows.Point startingOffset;

        // grab the image to pan it
        private void grabImage(object sender, MouseButtonEventArgs e)
        {
            if (sender is Border border)
            {
                if (border.Child is Image image)
                {
                    image.CaptureMouse();

                    grabbingPoint = e.GetPosition(border);
                    startingOffset.X = image.RenderTransform.Value.OffsetX;
                    startingOffset.Y = image.RenderTransform.Value.OffsetY;
                }
            }
        }

        // pan the image
        private void panImage(object sender, MouseEventArgs e)
        {
            if (sender is Border border)
            {
                if (border.Child is Image image)
                {
                    if (image.IsMouseCaptured)
                    {
                        System.Windows.Point mousePosition = e.GetPosition(border);

                        Matrix m = image.RenderTransform.Value;
                        m.OffsetX = startingOffset.X + (mousePosition.X - grabbingPoint.X);
                        m.OffsetY = startingOffset.Y + (mousePosition.Y - grabbingPoint.Y);

                        image.RenderTransform = new MatrixTransform(m);

                        fitToBounds(image);
                    }
                }
            }
        }

        // stop panning the image
        private void releaseImage(object sender, MouseButtonEventArgs e)
        {
            if (sender is Border border)
            {
                if (border.Child is Image image)
                {
                    image.ReleaseMouseCapture();
                }
            }
        }

        // save the image converted with the average algorithm
        private void saveAverageGS(object sender, RoutedEventArgs e)
        {
            save(gs1bm, "averageGS");
        }

        // save the image converted with the desaturation algorithm
        private void saveDesaturationGS(object sender, RoutedEventArgs e)
        {
            save(gs2bm, "desaturationGS");
        }

        // save the image converted with the luminosity algorithm
        private void saveLuminosityGS(object sender, RoutedEventArgs e)
        {
            save(gs3bm, "luminosityGS");
        }

        // resize the images along with the window
        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            fitToBounds(inputImage);
            fitToBounds(grayscale1);
            fitToBounds(grayscale2);
            fitToBounds(grayscale3);
        }
    }
}