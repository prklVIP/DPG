#ifndef DPG_INTEGRATORS_HPP
#define DPG_INTEGRATORS_HPP


/* Provides some commonly used integrators in DPG methods.

   The DPG integrators in pde files are defined on 
   compound spaces and have the form 
   
     ..........
     <form definition> -fespace=<compound> ...
     <dpg integrator name>  <ind1>  <ind2> ...
     ..........
     
   These integrators  make matrices of the forms of the type  
      
       b(u, v) = sum of integrals of C(u) * D(v) + Hermitian transpose

  where C and D are two (differential) operators,

       u is a function in <ind1> component of <compound> space
       v is a function in <ind2> component of <compound> space.
 */


#include <solve.hpp>

using namespace ngsolve;

namespace dpg {

  class DPGintegrator : public BilinearFormIntegrator  {

    shared_ptr<CoefficientFunction> comp1;
    shared_ptr<CoefficientFunction> comp2;
    int ind1;
    int ind2;

  public:
    
    DPGintegrator(const Array<shared_ptr<CoefficientFunction>> & coeffs) 
      : comp1(coeffs[0]), comp2(coeffs[1]) {

      if ( comp1->IsComplex() ) {
	  BaseMappedIntegrationPoint ip;
	  ind1 = int( comp1 -> EvaluateComplex(ip).real() ) - 1 ;
	  ind2 = int( comp2 -> EvaluateComplex(ip).real() ) - 1 ;
      }
      else {
	ind1 = int( comp1 -> EvaluateConst() ) - 1 ;
	ind2 = int( comp2 -> EvaluateConst() ) - 1 ;
      }
      
    }
    

    int GetInd1() const {return ind1;} 
    int GetInd2() const {return ind2;} 

  };


  /////////////////////////////////////////////////////////////////
  // Integrate a(x)*grad u . grad v, where u and v are in different spaces

  template<int D> class GradGrad : public DPGintegrator {
    
    shared_ptr<CoefficientFunction> coeff_a;

    template<class SCAL>
    void T_CalcElementMatrix (const FiniteElement & base_fel,
			      const ElementTransformation & eltrans, 
			      FlatMatrix<SCAL> elmat,
			      LocalHeap & lh)  const ;        
  public:
    
    GradGrad(const Array<shared_ptr<CoefficientFunction>> & coeffs) 
      : DPGintegrator(coeffs), coeff_a(coeffs[2])  {

      cout << "Using DPG integrator " << Name() << " with components "
	   << GetInd1()+1 << " and " << GetInd2()+1 << endl ; 
    }

    virtual xbool IsSymmetric() const { return !coeff_a->IsComplex() ; }
    
    virtual string Name () const { return "GradGrad"; }

    virtual bool BoundaryForm () const { return false; }

    virtual VorB VB() const { return VOL; }
    
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<double> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<double>(base_fel,eltrans,elmat,lh);
					       
    }
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<Complex> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<Complex>(base_fel,eltrans,elmat, lh);    
    }
			     

  };


  /////////////////////////////////////////////////////////////////
  // Integrate d(x) * q.n * v on all ELEMENT BOUNDARIES
  // where q is an Hdiv space and v is in a scalar space,
  // and d is a complex or real coefficient.

  template<int D> class FluxTrace : public DPGintegrator {
    
    shared_ptr<CoefficientFunction>  coeff_d;
    
    template<class SCAL>
    void T_CalcElementMatrix (const FiniteElement & base_fel,
			      const ElementTransformation & eltrans, 
			      FlatMatrix<SCAL> elmat,
			      LocalHeap & lh)  const ;
    
  public:
    
    FluxTrace(const Array<shared_ptr<CoefficientFunction>> & coeffs) 
      : DPGintegrator(coeffs), coeff_d(coeffs[2])  {

      cout << "Using DPG integrator " << Name() << " with components "
	   << GetInd1()+1 << " and " << GetInd2()+1 << endl ; 
    }

    virtual xbool IsSymmetric() const { return !coeff_d->IsComplex() ; }
    
    virtual string Name () const { return "FluxTrace"; }

    virtual bool BoundaryForm () const { return false; }

    virtual VorB VB() const { return VOL; }
    
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<double> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<double>(base_fel,eltrans,elmat,lh);
					       
    }
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<Complex> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<Complex>(base_fel,eltrans,elmat, lh);    
    }
  };

  /////////////////////////////////////////////////////////////////
  // Integrate a(x)* u * v, where u and v are in different spaces

  template<int D> class EyeEye : public DPGintegrator  {
    
    shared_ptr<CoefficientFunction> coeff_a;
    
    template<class SCAL>
    void T_CalcElementMatrix (const FiniteElement & base_fel,
			      const ElementTransformation & eltrans, 
			      FlatMatrix<SCAL> elmat,
			      LocalHeap & lh)  const ;

  public:
    
    EyeEye(const Array<shared_ptr<CoefficientFunction>> & coeffs) 
      : DPGintegrator(coeffs), coeff_a(coeffs[2])  {
    
      cout << "Using DPG integrator " << Name() << " with components "
	   << GetInd1()+1 << " and " << GetInd2()+1 << endl ; 
    }


    virtual xbool IsSymmetric() const { return !coeff_a->IsComplex() ; }
    
    virtual string Name () const { return "EyeEye"; }

    virtual bool BoundaryForm () const { return false; }
    virtual VorB VB() const { return VOL; }
    
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<double> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<double>(base_fel,eltrans,elmat,lh);
					       
    }
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<Complex> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<Complex>(base_fel,eltrans,elmat, lh);    
    }
  };

  /////////////////////////////////////////////////////////////////
  // Integrate c(x) * u * v over ELEMENT BOUNDARIES, where u and v 
  // are in different spaces

  template<int D> class TraceTrace : public DPGintegrator  {
    
    shared_ptr<CoefficientFunction> coeff_c;
    
    template<class SCAL>
    void T_CalcElementMatrix (const FiniteElement & base_fel,
			      const ElementTransformation & eltrans, 
			      FlatMatrix<SCAL> elmat,
			      LocalHeap & lh)  const ;

  public:
    
    TraceTrace(const Array<shared_ptr<CoefficientFunction>> & coeffs) 
      : DPGintegrator(coeffs), coeff_c(coeffs[2])  {

      cout << "Using DPG integrator " << Name() << " with components "
	   << GetInd1()+1 << " and " << GetInd2()+1 << endl ; 
    }

    virtual xbool IsSymmetric() const { return !coeff_c->IsComplex() ; }
    
    virtual string Name () const { return "TraceTrace"; }
    virtual bool BoundaryForm () const { return false; }
    virtual VorB VB() const { return VOL; }
    
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<double> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<double>(base_fel,eltrans,elmat,lh);
					       
    }
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<Complex> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<Complex>(base_fel,eltrans,elmat, lh);    
    }
  };


  /////////////////////////////////////////////////////////////////
  // Integrate c(x) * q.n * r.n over the GLOBAL BOUNDARY

  template<int D> 
  class FluxFluxBoundary : public DPGintegrator   {
    
    shared_ptr<CoefficientFunction> coeff_c;
    
    template<class SCAL>
    void T_CalcElementMatrix (const FiniteElement & base_fel,
  			      const ElementTransformation & eltrans, 
  			      FlatMatrix<SCAL> elmat,
  			      LocalHeap & lh)  const ; 
  public:

    FluxFluxBoundary(const Array<shared_ptr<CoefficientFunction>> & coeffs)
      : DPGintegrator(coeffs), coeff_c(coeffs[2]) {

      cout << "Using DPG integrator " << Name() << " with components "
	   << GetInd1()+1 << " and " << GetInd2()+1 << endl ; 
    }

    virtual xbool IsSymmetric() const { return !coeff_c->IsComplex() ; }
    
    virtual string Name () const { return "FluxFluxBoundary"; }
    virtual int DimElement () const { return D-1; }
    virtual int DimSpace () const { return D; }		
    virtual bool BoundaryForm () const { return 1; }
    virtual VorB VB() const { return BND; }
    
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<double> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<double>(base_fel,eltrans,elmat,lh);
					       
    }
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<Complex> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<Complex>(base_fel,eltrans,elmat, lh);    
    }
  };
  

  /////////////////////////////////////////////////////////////////
  // Integrate c(x) * u * e over the GLOBAL BOUNDARY, where u and e
  // are from different spaces in general. This works whenever the u
  // and the e spaces have "surface elements", i.e., elements on the
  // boundary facets that represent traces of u and e on the global
  // boundary. E.g., NGSolve's L2HighOrderFESpace do not have such
  // surface elements, but H1 spaces and L2HighOrderFESpaceTrace
  // do. (See also next integrator for a different approach.)

  template<int D> 
  class TraceTraceBoundary : public DPGintegrator   {
    
    shared_ptr<CoefficientFunction> coeff_c;
    
    template<class SCAL>
    void T_CalcElementMatrix (const FiniteElement & base_fel,
  			      const ElementTransformation & eltrans, 
  			      FlatMatrix<SCAL> elmat,
  			      LocalHeap & lh)  const ; 
  public:

    TraceTraceBoundary(const Array<shared_ptr<CoefficientFunction>> & coeffs)
      : DPGintegrator(coeffs), coeff_c(coeffs[2]) {

      cout << "Using DPG integrator " << Name() << " with components "
	   << GetInd1()+1 << " and " << GetInd2()+1 << endl ; 
    }

    virtual xbool IsSymmetric() const { return !coeff_c->IsComplex() ; }
    
    virtual string Name () const { return "TraceTraceBoundary"; }
    virtual int DimElement () const { return D-1; }
    virtual int DimSpace () const { return D; }		
    virtual bool BoundaryForm () const { return 1; }
    virtual VorB VB() const { return BND; }
    
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<double> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<double>(base_fel,eltrans,elmat,lh);
					       
    }
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<Complex> & elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<Complex>(base_fel,eltrans,elmat, lh);    
    }
  };

  /////////////////////////////////////////////////////////////////
  // The next integrator also integrates c(x) * u * e * ds over the
  // GLOBAL BOUNDARY. The difference is that now u and e are in finite
  // element spaces which may not have "surface elements" that
  // represent their global boundary traces. The idea is to integrate
  // on those facet elements of a volume element that lie on the
  // global boundary.
  //
  // This integrator works correctly with "-eliminate_internal" flag.
  //
  // This integrator currently does not work for general coefficients
  // (because we haven't implemented a mechanism to take a volume
  // coefficient and make it into a boundary coefficient) but it
  // works, for example, for constant coefficients.
  //
  template<int D> 
  class RobinVolume : public DPGintegrator   {
    
    shared_ptr<CoefficientFunction> coeff_c;

    template<class SCAL>
    void T_CalcElementMatrix (const FiniteElement & base_fel,
			      const ElementTransformation & eltrans, 
			      FlatMatrix<SCAL> elmat,
			      LocalHeap & lh)  const ;

  public:
 

    RobinVolume(const Array<shared_ptr<CoefficientFunction>> & coeffs)
      : DPGintegrator(coeffs), coeff_c(coeffs[2]) {

      cout << "Using DPG integrator " << Name() << " with components "
	   << GetInd1()+1 << " and " << GetInd2()+1 << endl ; 
    }

    virtual xbool IsSymmetric() const { return !coeff_c->IsComplex() ; }
    
    virtual string Name () const { return "RobinVolume"; }
    virtual int DimElement () const { return D; }
    virtual int DimSpace () const { return D; }		
    virtual bool BoundaryForm () const { return false; }
    virtual VorB VB() const { return VOL; }

    void CalcElementMatrix (const FiniteElement & base_fel,
    			    const ElementTransformation & eltrans, 
    			    FlatMatrix<double> elmat,
    			    LocalHeap & lh) const {
      
      T_CalcElementMatrix<double>(base_fel, eltrans, elmat, lh); 
    }
    
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<Complex> elmat,
			    LocalHeap & lh) const {

      T_CalcElementMatrix<Complex>(base_fel, eltrans, elmat, lh); 
    }
  };

  /////////////////////////////////////////////////////////////////
  // A source integrator (to accompany the last integrator) that
  // integrates (G.n + g) * e * ds over the GLOBAL BOUNDARY, where e
  // may be in a finite element space without "surface elements", and
  // g is a volume coefficient (which will be evaluated only on the
  // boundary). The integrator is called in this form in 2D
  //    neumannvol <ind> <g> <Gx> <Gy>  
  // and 
  //    neumannvol <ind> <g> <Gx> <Gy> <Gz>
  // in 3D. 

  // This integrator currently does not work for general coefficients
  // (because we haven't implemented a mechanism to take a volume
  // coefficient and make it into a boundary coefficient).
  //

  template<int D> 
  class NeumannVolume : public LinearFormIntegrator   {
    
    shared_ptr<CoefficientFunction> coeff_index;
    shared_ptr<CoefficientFunction> coeff_g;
    shared_ptr<CoefficientFunction> coeff_Gx;
    shared_ptr<CoefficientFunction> coeff_Gy;
    shared_ptr<CoefficientFunction> coeff_Gz;

    template<class SCAL>
    void T_CalcElementVector (const FiniteElement & base_fel,
			      const ElementTransformation & eltrans, 
			      FlatVector<SCAL> elvec,
			      LocalHeap & lh)  const ;
    int indx; 

  public: 

    NeumannVolume(const Array<shared_ptr<CoefficientFunction>> & coeffs) 
      : coeff_index(coeffs[0]), coeff_g(coeffs[1]), 
	coeff_Gx(coeffs[2]), coeff_Gy(coeffs[3]), coeff_Gz(coeffs[4]) {

      indx = int( coeff_index -> EvaluateConst() ) - 1 ;
      
      cout << "Using DPG source integrator " << Name() 
	   << " on component " << indx+1 << endl ; 
    }
							       
    virtual string Name () const { return "NeumannVolume"; }
    virtual int DimElement () const { return D; }
    virtual int DimSpace () const { return D; }		
    virtual bool BoundaryForm () const { return false; }
    virtual VorB VB() const { return VOL; }

    void CalcElementVector (const FiniteElement & base_fel,
    			    const ElementTransformation & eltrans, 
    			    FlatVector<double> elvec,
    			    LocalHeap & lh) const {
      
      T_CalcElementVector<double>(base_fel, eltrans, elvec, lh); 
    						
    }
    
    void CalcElementVector (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatVector<Complex> elvec,
			    LocalHeap & lh) const {

      T_CalcElementVector<Complex>(base_fel, eltrans, elvec, lh); 

    }
  };


  /////////////////////////////////////////////////////////////////
  // Integrate c(x) * q.n * w over the GLOBAL BOUNDARY

  template<int D> 
  class FluxTraceBoundary : public DPGintegrator   {
    
    shared_ptr<CoefficientFunction> coeff_c;
    
    template<class SCAL>
    void T_CalcElementMatrix (const FiniteElement & base_fel,
  			      const ElementTransformation & eltrans, 
  			      FlatMatrix<SCAL> elmat,
  			      LocalHeap & lh)  const ; 
  public:

    FluxTraceBoundary(const Array<shared_ptr<CoefficientFunction>> & coeffs)
      : DPGintegrator(coeffs), coeff_c(coeffs[2]) {

      cout << "Using DPG integrator " << Name() << " with components "
	   << GetInd1()+1 << " and " << GetInd2()+1 << endl ; 

    }

    virtual xbool IsSymmetric() const { return !coeff_c->IsComplex() ; }
    virtual string Name () const { return "FluxTraceBoundary"; }
    virtual int DimElement () const { return D-1; }
    virtual int DimSpace () const { return D; }		
    virtual bool BoundaryForm () const { return 1; }
    virtual VorB VB() const { return BND; }
    
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<double> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<double>(base_fel,eltrans,elmat,lh);
					       
    }
    void CalcElementMatrix (const FiniteElement & base_fel,
			    const ElementTransformation & eltrans, 
			    FlatMatrix<Complex> elmat,
			    LocalHeap & lh) const {
      T_CalcElementMatrix<Complex>(base_fel,eltrans,elmat, lh);    
    }
  };



  /////////////////////////////////////////////////////////////////

}

#endif
