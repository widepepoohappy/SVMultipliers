eval `envmgr -p PATH /pkg/qvmr/bin`


#setenv WORKSPACE		     /usr2/doconnor/QRV/qrv_prj
#setenv WORKSPACE		     /usr2/dgehin/design_perso/QRV/qrv_prj
setenv WORKSPACE		     $cwd
#echo $WORKSPACE
setenv QVMR_PATH		     ${WORKSPACE}/verif/top/tool_setup/qvmr

setenv scratch		        /usr2/cclement/scratch

#setenv unmanaged		     ${WORKSPACE}/unmanaged
#setenv include               ${WORKSPACE}/qrv/include
#setenv design_deps           ${WORKSPACE}/qrv/sub
setenv sv_src                   ${WORKSPACE}/sv
setenv tb                    ${WORKSPACE}/verif
#setenv verif_scripts         ${WORKSPACE}/verif/sim/scripts
#setenv cores			     ${design_deps}
#setenv sub			         ${design_deps}
#setenv qtmr			         ${design_deps}/qtmr
#setenv coresight_components	 ${design_deps}/coresight_components
#setenv cdc_comps		     ${design_deps}/cdc_comps
#setenv cdc_comp_assertions	 ${cdc_comps}/assertions
#setenv clk_gate_wlan		 ${design_deps}/clk_gate
	 
setenv LM_LICENSE_FILE		 `/pkg/qct/software/v2p/bin/v2p synopsys novas`
#setenv pldrc           ${WORKSPACE}/impl/pldrc
#setenv cdc             ${WORKSPACE}/impl/cdc
#setenv uflow           ${WORKSPACE}/qrv/core_info
#setenv synth           ${WORKSPACE}/impl/synth
setenv DVT_VERSION  20.1.44
setenv IEEE_TARGETS /pkg/hwlibraries/behavior/ieee/generic/Makefile.Targets

setenv VCS_VERSION       vcs-mx_vT-2022.06-1
#setenv VCS_VERSION       vcs-mx_vS-2019.09

setenv VCS_HOME 		/pkg/qct/software/synopsys/vcs/${VCS_VERSION}/

setenv UVM_HOME			     /pkg/qct/software/dv_meth/uvm/uvm-1.1d

setenv DRM_PROJECT `/pkg/icetools/bin/ptagger -f 71690 -t 02 -g eda`
setenv SNPSLMD_LICENSE_FILE `/pkg/qct/licenses/tools/bin/vendor2port.pl -vendor synopsys -feature VCSRuntime_Net`
#setenv EUCLIDE_HOME /pkg/qct/software/synopsys/euclide/euclide_vU-2023.03/
# Modelsim license and version
setenv LM_LICENSE_FILE `/pkg/licenses/tools/bin/vendor2port.pl mentor`
setenv MODELTECH_VERSION 2020.4_3

