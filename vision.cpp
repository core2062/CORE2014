#include "vision.h"	

PixelValue pixVal(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
		PixelValue ret;
		ret.grayscale = 14145495;
		ret.rgb.R = r;
		ret.rgb.G = g;
		ret.rgb.B = b;
		ret.rgb.alpha = a;
		cout << (int)ret.rgb.R << " " << (int)ret.rgb.G << " " << (int)ret.rgb.B << endl;
		return ret;
	}
	
#define chk(input) {if(!(input)) {goto error;}}
#define log(im) {cout<<"writing "<<step<<endl; (im).Write(("/."+step+".bmp").c_str());}
	
	void CORE::visionMain(){
		double minArea = 8.0;
		double minThreshold = 2;
		bool hot = false;
		bool isHoriz = false;
		bool isVertical = false;
		cout << "starting autonomous" << endl;
		AxisCamera& camera = AxisCamera::GetInstance("10.20.62.11");
		camera.WriteBrightness(10);
		std::string step = "start";
		while(!camera.IsFreshImage()){
			// pass
		}
		ColorImage* image = camera.GetImage();
		log(*image);
		
		step = "thresh";
		BinaryImage* thresholdImage = image->ThresholdRGB(0, 22, 28, 195, 0, 71);
//		BinaryImage* thresholdImage = image->ThresholdRGB(0,224,217,255,140,255);
		log(*thresholdImage);
		
		step = "small";
		BinaryImage* small = thresholdImage->RemoveSmallObjects(true, 1);
		log(*small);
		
		vector<ParticleAnalysisReport>* reports = small->GetOrderedParticleAnalysisReports();
		vector<ParticleAnalysisReport>::const_iterator i;
		for(i = reports->begin(); i!=reports->end(); i++){
			cout<< i->boundingRect.top <<" "<< i->boundingRect.left << endl;
			if (i->particleArea > minArea){
				cout << "passes area" << endl;
				if ((i->boundingRect.width / i->boundingRect.height)> minThreshold){
					isHoriz = true;
					cout<< "horizontal seen"<<endl;
				} else if((i->boundingRect.height / i->boundingRect.width)>minThreshold){
					isVertical =true;
					cout<< "Vertical seen"<<endl;
				}
				if (isVertical && isHoriz){
					hot = true;
					cout<< "hot " << hot <<endl;
				}
			}
		}
		
		delete image;
		delete thresholdImage;
		delete small;
		delete reports;
		camera.WriteBrightness(50);
		while(!camera.IsFreshImage()){
			// pass
		}
		AxisCamera::DeleteInstance();
	}
	void CORE::TestSubtraction()
	{
		ColorImage image (IMAQ_IMAGE_RGB);
		ColorImage output (IMAQ_IMAGE_RGB);
		Image* imaqImage = image.GetImaqImage();
		Image* imaqOutput = output.GetImaqImage();
		
		std::string step = "start";
		chk(imaqReadFile(imaqImage, "/Center.jpg", NULL, NULL));
		chk(imaqReadFile(imaqOutput, "/Center.jpg", NULL, NULL));
		log(image);
		
		PixelValue darkening_cnst;
		
		step = "red";
		darkening_cnst = pixVal(255, 0, 0, 0);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		
		step = "green";
		darkening_cnst = pixVal(0, 255, 0, 0);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		
		step = "blue";
		darkening_cnst = pixVal(0, 0, 255, 0);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		
		step = "alpha";
		darkening_cnst = pixVal(0, 0, 0, 255);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		
		step = "grey";
		darkening_cnst = pixVal(215, 215, 215, 0);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		
		return;
		error:
			cout << "Error in step " << step << endl;
	}
