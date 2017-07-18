#!/bin/bash
if [ -z $1 ];then
  echo "no MGW_NAME given"
  exit 1
fi
source /root/BS/mgw_setting.cnf

echo "---restart CLA-0 RU start---"
MGW_NAME=$1
case $MGW_NAME in
  Anna)
  MGW_IPNIURG0=$Anna_MGW_IPNIURG0
  UP_MASK=$Anna_UP_MASK
  ;;
  Catherine)
  MGW_IPNIURG0=$Catherine_MGW_IPNIURG0
  UP_MASK=$Catherine_UP_MASK
  ;;
  Kaka)
  MGW_IPNIURG0=$Kaka_MGW_IPNIURG0
  UP_MASK=$Kaka_UP_MASK
  ;;
  Romeo)
  MGW_IPNIURG0=$Romeo_MGW_IPNIURG0
  UP_MASK=$Romeo_UP_MASK
  ;;
  *)
  echo "no name found"
  ;;
esac


fshascli -rn /CLA-0/MGW_OMUFU-0
sleep 4
fshascli -s /CLA-0/MGW_OMUFU-0

fshascli -rn /CLA-0/MGW_CMFU-0
sleep 4
fshascli -s /CLA-0/MGW_CMFU-0

fshascli -rn /CLA-0/MGW_HCLBFU-*
sleep 4
fshascli -s /CLA-0/MGW_HCLBFU-*

fshascli -rn /CLA-0/MGW_SISUFU-*
sleep 4
fshascli -s /CLA-0/MGW_SISUFU-*
echo "---restart CLA-0 RU end---"

echo "---UP init work around start---"
fsclish -c "delete networking address dedicated /MGW_IPNIURG-0 iface eth_sfp1 ip-address $MGW_IPNIURG0"
fsclish -c "add networking address dedicated /MGW_IPNIURG-0 iface eth_sfp1 ip-address $MGW_IPNIURG0/$UP_MASK"
echo "---UP init work around end---"
