# You can use LHE files from scikit-hep-testdata
# from skhep_testdata import data_path
import argparse
import numpy as np
import matplotlib.pyplot as plt
import awkward as aw
import vector 
vector.register_awkward()

"""
Note  : 
    Muons are considered massless here.
    All observables are in CM frame as of now

"""

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i',"--input", help="Input file")
    parser.add_argument('-t',"--tag", help="Tag, added as prefix to the plot filenames",default="hw")
    args = parser.parse_args()

    print("Loading  : ",args.input)
    with open(args.input) as f:
        txt=f.readlines()
    data={
        'mu1':{'pt':[],'eta' : [] ,'phi' : [] },
        'mu2':{'pt':[],'eta' : [] ,'phi' : [] }
    }
    for l in txt:
        items=l[:-1].split(',')[1:]
        pt1,pt2=float(items[0]),float(items[3])
        t1,t2='mu1','mu2'
        if pt2>pt1: t1,t2='mu2','mu1'
            
        data[t1]['pt'].append(float(items[0]))
        data[t1]['eta'].append(float(items[1]))
        data[t1]['phi'].append(float(items[2]))
        data[t2]['pt'].append(float(items[3]))
        data[t2]['eta'].append(float(items[4]))
        data[t2]['phi'].append(float(items[5]))

    for k in data:
        for l in data[k]:
            data[k][l]=np.array(data[k][l])
    mu1=aw.zip({'pt':data['mu1']['pt'],'eta': data['mu1']['eta'],'phi':data['mu1']['phi'],'mass':0.0},
               with_name="Momentum4D")
    mu2=aw.zip({'pt':data['mu2']['pt'],'eta': data['mu2']['eta'],'phi':data['mu2']['phi'],'mass':0.0},
               with_name="Momentum4D")
    mumu=mu1+mu2

    plt.figure()
    _=plt.hist(data['mu1']['pt'],bins=np.arange(0.0,100,1.0),color='r',label='Lead $\mu$')
    _=plt.hist(data['mu2']['pt'],bins=np.arange(0.0,100,1.0),color='b',label='Sublead $\mu$')
    plt.xlabel("pT [GeV]",loc='right')
    plt.legend()
    fname=f'{args.tag}_pt.png'
    print(f"Saving {fname}")
    plt.savefig(fname,bbox_inches='tight',dpi=150)

    plt.figure()
    _=plt.hist(data['mu1']['eta'],bins=np.arange(-4.0,4.01,0.10),color='r',label='Lead $\mu$')
    _=plt.hist(data['mu2']['eta'],bins=np.arange(-4.0,4.01,0.10),color='b',label='Sublead $\mu$')
    plt.xlabel("$\eta$",loc='right')
    plt.legend()
    fname=f'{args.tag}_eta.png'
    print(f"Saving {fname}")
    plt.savefig(fname,bbox_inches='tight',dpi=150)

    plt.figure()
    _=plt.hist(data['mu1']['phi'],bins=np.arange(-0.0,7.01,0.10),color='r',label='Lead $\mu$')
    _=plt.hist(data['mu2']['phi'],bins=np.arange(-0.0,7.01,0.10),color='b',label='Sublead $\mu$')
    plt.xlabel("$\phi$",loc='right')
    plt.legend()
    fname=f'{args.tag}_phi.png'
    print(f"Saving {fname}")
    plt.savefig(fname,bbox_inches='tight',dpi=150)

    plt.figure()
    _=plt.hist(mumu.mass,bins=np.arange(0.0,180.0,1.0),color='b',label='mass($\mu\mu$)')
    plt.xlabel("m$_{\mu\mu}$ [ GeV ]")
    plt.legend()
    fname=f'{args.tag}_mass.png'
    print(f"Saving {fname}")
    plt.savefig(fname,bbox_inches='tight',dpi=150)

if __name__=='__main__':
    main()
