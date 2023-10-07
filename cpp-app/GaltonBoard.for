C     PROGRAMA PARA INTEGRAR ECUACIONES DIFERENCIALES
       PARAMETER (N=1,NS=100,NP=100,No=1,Nis=100)
       PARAMETER(NR=1000,NPT=500000,ncut=1,Ro=2,alphaR=0.4,alphaO=0.4)
       COMMON Temp,Gamma,Gsed,sigma,eps
       Real x(NP),y(NP),Vx(NP),Vy(NP),xo(NS),yo(NP),Vxo(NP),Vyo(NP) 
  !     Real xoo(NP),yoo(NP),Vxoo(NP),Vyoo(NP)
       Real DYDTvx(NP),DYDTvy(NP),fx(Np),fy(Np),raux,de,dy,vr,vt
       real xs(3*NS+1),ys(3*Ns+1),per,Lx,Ly,xini,xc(No),yc(No)
       Real vxa,vya,Hi(Nis),Hit(Nis)
        integer*8 npts,mynpts
        integer ierr, myid, nprocs,Ji,l
    !   Integer Lx,Ly
       include "mpif.h"
       open (8, file='posP.dat')
       open (9, file='His.dat')
       call MPI_INIT(ierr)
       call MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)
       call MPI_COMM_SIZE(MPI_COMM_WORLD, nprocs, ierr)         
        

  !     INTEGER*2   rseed,s
       H=0.005
       T=0
       Gsed=1.0
       Lx=float(NS)
       Ly=Float(NS)
       Hi=0
       Hit=0
       npts=NR
        if (myid .eq. 0) then
           mynpts = npts - (nprocs-1)*(npts/nprocs)
        else
           mynpts = npts/nprocs
        endif
       xs=0.0
       ys=0.0
!      Centro de los obstaculos  
       do i=1,No
       xc(i)=0.5*Lx
       yc(i)=0.5*Ly
       continue 
       end do 
!      Base de la caja
       Do i=1,NS
       xs(i)=((i-1)+0.5)
       ys(i)=0. 
       Continue
       End do
!      Paredes de la caja
!      Pared Derecha  
       Do i=Ns+1,2*NS
       xs(i)=(Ns-1)+0.5
       ys(i)=((i-Ns)-1)+0.5
       Continue
       End do

!      Pared izquierda
       Do i=2*NS+1,3*NS
       xs(i)=(0.5)
       ys(i)=(((i-2*NS)-1)+0.5) 
       Continue
       End do
       
       do ji=1,mynpts
       idum=-ji+myid*mynpts
       NPa=0
!      Itera en el tiempo
       do J=1,NPT
!      Deposita las partículas cada cierto tiempo      
       if(J/float(1000).eq.int(J/float(1000)))then
       Npa=Npa+1
       if(Npa.gt.NP)then
       goto 10
       end if
       x(Npa)=0.5*Lx+sigma*0.5
       y(Npa)=Ly-1
       Vx(Npa)=2.5*(ran2(idum)-0.5)
       Vy(Npa)=2.5*(ran2(idum)-0.5)
       end if


!      Integra las EC de Mov
       do i=1, Npa
        vx(i)=vx(i)
        if(y(i).gt.0)then
        vy(i)=vy(i)-gsed*H
        end if       
        x(i)=x(i)+vx(i)*H
        y(i)=y(i)+vy(i)*H
!     Colision con la base
       if(y(i).lt.0.0)then 
       vx(i)=vx(i)*alphaR*0.1
       vy(i)=-vy(i)*alphaR
       y(i)=0.0      
       end if   
!     Colision con la pared izquierda
        if(x(i).lt.0.5)then 
       vx(i)=abs(vx(i))*alphaR
       vy(i)=vy(i)!*alphaR
       x(i)=0.5 
       end if   
!     Colision con la pared derecha
        if(x(i).gt.Ns-0.5)then 
       vx(i)=-abs(vx(i))*alphaR
       vy(i)=vy(i)!*alphaR
       x(i)=Ns-0.5
       end if   

!      Colision con los obstaculos
       do l=1,No
       raux=sqrt((xc(l)-x(i))**2+(yc(l)-y(i))**2)
       dx=-xc(l)+x(i)
       dy=-yc(l)+y(i)
       vxa=vx(i)
       vya=vy(i)
       if(raux.lt.Ro)then
       vr=-alphaO*(vxa*dx+vya*dy)/raux
       vt=(-vxa*dy+vya*dx)/raux
       vx(i)=vr*dx/raux-vt*dy/raux
       vy(i)=vr*dy/raux+vt*dx/raux
       x(i)=Ro*dx/raux+xc(l)
       y(i)=Ro*dy/raux+yc(l)
       end if 
       continue
       end do 


       continue
       end do 
 
!      Escribe el archivo de ovito
       if(J/float(10).eq.int(J/float(10)).and.NR.eq.1)then
       write(8,*) 3*Ns+NPa+No
       write(8,*) 'aver' 
       do i=1,3*NS 
       write(8,*) i,2,xs(i),ys(i),1
       continue
       end do
       do i=1,NPa 
       write(8,*) 3*Ns+i,1,x(i),y(i),1
       continue
       end do
       do l=1,No
       write(8,*) 3*Ns+Npa+i,3,xc(l),yc(l),1
       continue 
       end do
       end if
       
       Continue
       end do !Ciclo tiempo
 

 10    continue !Ciclo repeticiones
	       do i=1,Npa
	       Hi(int(x(i))+1)=Hi(int(x(i))+1)+1
	       continue
	       end do 
       end do

        do i=1,Nis
        call MPI_REDUCE(Hi(i),Hit(i),1,MPI_Real,MPI_SUM,
     &     0,MPI_COMM_WORLD,ierr)
        continue
        end do  
      
       if (myid .eq. 0) then
       do i=1,Nis 
       write(9,*) i-0.5,Hit(i)/sum(Hit)
       continue
       end do
       end if

       call MPI_FINALIZE(ierr)
       
       END

        FUNCTION gasdev(idum)
	INTEGER idum
	REAL gasdev
	INTEGER iset
	REAL fac,gset,rsq,v1,v2,ran2
	SAVE iset,gset
	DATA iset/0/
	if (iset.eq.0) then
1       v1=2.*ran2(idum)-1
	v2=2.*ran2(idum)-1
	rsq=v1**2+v2**2 
	if(rsq.ge.1..or.rsq.eq.0.)goto 1 
	fac=sqrt(-2.*log(rsq)/rsq) 
	gset=v1*fac
	gasdev=v2*fac
	iset=1 
	else 
	gasdev=gset 
	iset=0 
	endif
	return
	END


        FUNCTION RAN2(IDUM)
        INTEGER idum,IM1,IM2,IMM1,IA1,IQ1,IQ2,IR1,IR2,NTAB,NDIV
	REAL RAN2,AM,EPS,RNMX
	PARAMETER (IM1=2147483563,IM2=2147483399,AM=1./IM1,IMM1=IM1-1)
	PARAMETER (IA1=40014,IA2=40692,IQ1=53688,IQ2=52744,IR1=12211)
	PARAMETER(IR2=3791,NTAB=32,NDIV=1+IMM1/NTAB,EPS=1.2e-7)
	PARAMETER(RNMX=1.-EPS)
	INTEGER idum2,j,k,iv(NTAB),iy
	SAVE iv,iy,idum2
	DATA idum2/123456789/,iv/NTAB*0/,iy/0/
	if(idum.le.0)then
	idum=max(-idum,1)
	idum2=idum
	do j=NTAB+8,1,-1
		k=idum/IQ1
		idum=IA1*(idum-k*IQ1)-k*IR1
		if (idum.lt.0) idum=idum+IM1
		if (j.le.NTAB) iv(j)=idum
	end do
	iy=iv(1)
	end if
	k=idum/IQ1
	idum=IA1*(idum-k*IQ1)-k*IR1
	if (idum.lt.0) idum=idum+IM1
	k=idum2/IQ2
	idum2=IA2*(idum2-k*IQ2)-k*IR2
	if (idum2.lt.0) idum2=idum2+IM2
	j=1+iy/NDIV
	iy=iv(j)-idum2
	iv(j)=idum
	if(iy.lt.1)iy=iy+IMM1
	ran2=min(AM*iy,RNMX)
	return
	end

