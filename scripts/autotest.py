import argparse
import os
import sys
import utils

parser = argparse.ArgumentParser()
parser.add_argument('--input', type=str, required=True)

def main():

    # setup
    args = parser.parse_args()
    exp_path = os.path.join('./', 'experiments')
    try:
        os.mkdir(exp_path)
    except:
        pass
    
    #build
    os.chdir('./src/imaging/')
    os.system('make processmake')
    os.chdir('../../')
    
    # convs
    conv_params = {
        'size' : [32, 64, 128, 256, 512],
        'kernel':[3, 5, 7, 9],
        'sigma': [0.5, 0.85, 1.25]
    }
    
    dft_params = {
        'size' : [32, 64, 128],
        'r': [0.3, 1, 3]
    }
    
    fft_params = {
        'size' : [32, 64, 128, 256, 512],
        'r': [0.3, 1, 3]
    }
    
    # convs!
    for size in conv_params['size']:
        for kernel in conv_params['kernel']:
            for sigma in conv_params['sigma']:
                
                # build exp dir
                dir_path = os.path.join(exp_path, 'conv_'+str(size)+'_'+str(kernel)+'_'+str(sigma))
                try:
                    os.mkdir(dir_path)
                except:
                    pass
                
                # preprocess
                preprocess_arg = "python ./scripts/preprocess.py --input "+args.input+" --output "+dir_path+" --size "+str(size)
                os.system(preprocess_arg)
                
                # process
                conv_path = "./src/imaging/processmake -c "+dir_path + "/ noise.txt "+str(size)+" "+str(sigma)+" "+str(kernel)
                os.system(conv_path)
                
                # postprocess
                os.system("python ./scripts/postprocess.py --input " + dir_path)
                
    # dft!
    for size in dft_params['size']:
        for r in dft_params['r']:
                # build exp dir
                dir_path = os.path.join(exp_path, 'dft'+str(size)+'_'+str(r))
                try:
                    os.mkdir(dir_path)
                except:
                    pass
                
                # preprocess
                preprocess_arg = "python ./scripts/preprocess.py --input "+args.input+" --output "+dir_path+" --size "+str(size)
                os.system(preprocess_arg)
                
                # process
                conv_path = "./src/imaging/processmake -f "+dir_path + "/ noise.txt "+str(size)+" "+str(r)
                os.system(conv_path)
                
                # postprocess
                os.system("python ./scripts/postprocess.py --input " + dir_path)
                
    # fft!
    for size in fft_params['size']:
        for r in fft_params['r']:
                # build exp dir
                dir_path = os.path.join(exp_path, 'fft'+str(size)+'_'+str(r))
                try:
                    os.mkdir(dir_path)
                except:
                    pass
                
                # preprocess
                preprocess_arg = "python ./scripts/preprocess.py --input "+args.input+" --output "+dir_path+" --size "+str(size)
                os.system(preprocess_arg)
                
                # process
                conv_path = "./src/imaging/processmake -f "+dir_path + "/ noise.txt "+str(size)+" "+str(r)
                os.system(conv_path)
                
                # postprocess
                os.system("python ./scripts/postprocess.py --input " + dir_path)
                
if __name__ == '__main__':
    main()
                        
                
