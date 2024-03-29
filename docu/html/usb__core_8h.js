var usb__core_8h =
[
    [ "DeviceDescriptor", "usb__core_8h.html#structDeviceDescriptor", [
      [ "len", "usb__core_8h.html#afe03e417b85bfd6861136b34c8fbe5c1", null ],
      [ "dtype", "usb__core_8h.html#a2fe2871f294c36ffdb7f034ea660a8f8", null ],
      [ "usbVersion", "usb__core_8h.html#ab40e16638fb892e4692b843ff2efde35", null ],
      [ "deviceClass", "usb__core_8h.html#a3fe1d9631e3f06c76a1e96a92b3c5c68", null ],
      [ "deviceSubClass", "usb__core_8h.html#aeb7317ea9a0a0bc94dc7aa565c1c4148", null ],
      [ "deviceProtocol", "usb__core_8h.html#a2a4aa69baf122782e76f23a95b9246df", null ],
      [ "packetSize0", "usb__core_8h.html#a28369dc0e13d2588475459d1e10fbafa", null ],
      [ "idVendor", "usb__core_8h.html#a7d664f8953f46f07de11d5e7618ac5cb", null ],
      [ "idProduct", "usb__core_8h.html#a13717d21af8336ef8f22371ae0f728c6", null ],
      [ "deviceVersion", "usb__core_8h.html#aaf837badf84df90e58d999b27869276b", null ],
      [ "iManufacturer", "usb__core_8h.html#aec6908b21fad79901a7c70ac3130eaf3", null ],
      [ "iProduct", "usb__core_8h.html#ad6387eda44dae34603ab54960f9f158f", null ],
      [ "iSerialNumber", "usb__core_8h.html#a320a5fb0837292cee7485a1d8d8bcd12", null ],
      [ "bNumConfigurations", "usb__core_8h.html#a50bb8119059578840fdd8d7e3b2d1ec3", null ]
    ] ],
    [ "ConfigDescriptor", "usb__core_8h.html#structConfigDescriptor", [
      [ "len", "usb__core_8h.html#a89ee0a0d97191665e61e3605f416dc8b", null ],
      [ "dtype", "usb__core_8h.html#ae7ce820c169f678f10256b785f4d2af4", null ],
      [ "clen", "usb__core_8h.html#a92f0ebfd7eb691d8532fed649f3629c0", null ],
      [ "numInterfaces", "usb__core_8h.html#a00c4064d2b4d5d3f2d125d47e3d48efc", null ],
      [ "config", "usb__core_8h.html#a7aa1335be8a9abba392aa65900f55f91", null ],
      [ "iconfig", "usb__core_8h.html#afbf4f5586035340174072451ba8b07d8", null ],
      [ "attributes", "usb__core_8h.html#aefe0f0aeef665dea4ef0bbcba52fc962", null ],
      [ "maxPower", "usb__core_8h.html#ae0fcf7ce36ae8a8c3f09ecff3a3bfdca", null ]
    ] ],
    [ "InterfaceDescriptor", "usb__core_8h.html#structInterfaceDescriptor", [
      [ "len", "usb__core_8h.html#a0b4591e089b56e1ac6738c49fb790bfd", null ],
      [ "dtype", "usb__core_8h.html#a6fabe9de320b3b13171e3c17f8c55dcf", null ],
      [ "number", "usb__core_8h.html#af5fdca2676f2d4f31211b34ecfd23fc4", null ],
      [ "alternate", "usb__core_8h.html#a1a66fe749463e78c821dba68cc890cf4", null ],
      [ "numEndpoints", "usb__core_8h.html#ad26d84894390a3aea12c85bbde14abea", null ],
      [ "interfaceClass", "usb__core_8h.html#ac04b9fce481f841b45afb734e00062ef", null ],
      [ "interfaceSubClass", "usb__core_8h.html#a57f4fe6d3c3caf587c9ce6d351073cad", null ],
      [ "protocol", "usb__core_8h.html#a58d7f8d1315711c3e4430f3913275c38", null ],
      [ "iInterface", "usb__core_8h.html#ad1c332273a83571a85b04ff3c9e3324e", null ]
    ] ],
    [ "EndpointDescriptor", "usb__core_8h.html#structEndpointDescriptor", [
      [ "len", "usb__core_8h.html#ab9a710bc7df714b9d57795d1017deb27", null ],
      [ "dtype", "usb__core_8h.html#a66f0f0c5442c7195f5b0aee1ec7b8e0a", null ],
      [ "addr", "usb__core_8h.html#aa3db9199cbb8cc96c7233232ac666ce8", null ],
      [ "attr", "usb__core_8h.html#a9aace7000d55efabc963cd64a853622d", null ],
      [ "packetSize", "usb__core_8h.html#a6d55d0cc5884ab6358635f4bd25e5546", null ],
      [ "interval", "usb__core_8h.html#a90107c36875ecd8a27b51e2e43ed558e", null ]
    ] ],
    [ "LineInfo", "usb__core_8h.html#structLineInfo", [
      [ "dwDTERate", "usb__core_8h.html#aaacea931f4c9ce6ce0a19de7bbf4d7a4", null ],
      [ "bCharFormat", "usb__core_8h.html#a6765d6bd8842b04a9d9ba95fafd8eed2", null ],
      [ "bParityType", "usb__core_8h.html#a326c61e20ef195ac5a03fb38fc0fb209", null ],
      [ "bDataBits", "usb__core_8h.html#a3674fe987d34cae754776871a3d905d7", null ],
      [ "lineState", "usb__core_8h.html#a44d70a829c9116fd24013e0bd0b67aad", null ]
    ] ],
    [ "IADDescriptor", "usb__core_8h.html#structIADDescriptor", [
      [ "len", "usb__core_8h.html#a97b5a8ab3ec135dd34df1e8e886ff90d", null ],
      [ "dtype", "usb__core_8h.html#a6f5e4a25153d6d0353b4ab34bc214ec2", null ],
      [ "firstInterface", "usb__core_8h.html#a1cd9d563a84bec156eb9930ed7c7ae03", null ],
      [ "interfaceCount", "usb__core_8h.html#a00f35949eb9126126545df1d99214815", null ],
      [ "functionClass", "usb__core_8h.html#a1bd66d62627a406591db705dec378cfd", null ],
      [ "funtionSubClass", "usb__core_8h.html#ae700db4d37744cd89e5d236c0d4359d4", null ],
      [ "functionProtocol", "usb__core_8h.html#a833566d1402ea4414caa03a1a391b76e", null ],
      [ "iInterface", "usb__core_8h.html#a93037917079d72f2fcaa2501e9087c62", null ]
    ] ],
    [ "CDCCSInterfaceDescriptor", "usb__core_8h.html#structCDCCSInterfaceDescriptor", [
      [ "len", "usb__core_8h.html#a0d660e198aaad1670774a1b7c06d7b06", null ],
      [ "dtype", "usb__core_8h.html#a58252d8ce8fec66e8c0d56a21783ff82", null ],
      [ "subtype", "usb__core_8h.html#af1f0190c16f15176447e48965080e517", null ],
      [ "d0", "usb__core_8h.html#a79837689bbe63fbaf89a96ab944d018b", null ],
      [ "d1", "usb__core_8h.html#a211f930fce407c3aeb1d1451edbaf03a", null ]
    ] ],
    [ "CDCCSInterfaceDescriptor4", "usb__core_8h.html#structCDCCSInterfaceDescriptor4", [
      [ "len", "usb__core_8h.html#ad0f08039cd9133404e76e40b85845eae", null ],
      [ "dtype", "usb__core_8h.html#ab9e69691ce3e4ae88a2c22b5e2d54b7f", null ],
      [ "subtype", "usb__core_8h.html#aec378d652499b31029ac34cb02835f68", null ],
      [ "d0", "usb__core_8h.html#ab58b372c9d590078f9a179577d2dab50", null ]
    ] ],
    [ "CMFunctionalDescriptor", "usb__core_8h.html#structCMFunctionalDescriptor", [
      [ "len", "usb__core_8h.html#aabce0141ac81f5e3ac36828c814df019", null ],
      [ "dtype", "usb__core_8h.html#a5f323d6dcdf0ea1be095203e98365984", null ],
      [ "subtype", "usb__core_8h.html#a651ba43f14010569114ca43d9b92c641", null ],
      [ "bmCapabilities", "usb__core_8h.html#a4633e21f38d5f823af1d74128611f185", null ],
      [ "bDataInterface", "usb__core_8h.html#a318ef2bc66d8983c9cc35720cf788126", null ]
    ] ],
    [ "ACMFunctionalDescriptor", "usb__core_8h.html#structACMFunctionalDescriptor", [
      [ "len", "usb__core_8h.html#a40d19124b817dbe40f6c7f77df96eb64", null ],
      [ "dtype", "usb__core_8h.html#ac806f76305b3c9f083db638d9558765a", null ],
      [ "subtype", "usb__core_8h.html#a3fd32bf39f6ea0df5c4db59fa9face25", null ],
      [ "bmCapabilities", "usb__core_8h.html#a0430c341c87f137082ccb8ee92a1cadd", null ]
    ] ],
    [ "MSCDescriptor", "usb__core_8h.html#structMSCDescriptor", [
      [ "msc", "usb__core_8h.html#ad1df90234c1714bd3ffa34c0a28d4621", null ],
      [ "in", "usb__core_8h.html#a9ae4c4c01f7b1f9ed6c16bbd8793dfd9", null ],
      [ "out", "usb__core_8h.html#a2e9b0bbe81b8b510cd182c70560c33eb", null ]
    ] ],
    [ "CDCDescriptor", "usb__core_8h.html#structCDCDescriptor", [
      [ "iad", "usb__core_8h.html#a42c277dcdc03a000bb15a468104c996a", null ],
      [ "cif", "usb__core_8h.html#ad3afcf5687ec1055195bd94a1f36d269", null ],
      [ "header", "usb__core_8h.html#a3f84b78e1d78e821a539564e57375cf5", null ],
      [ "callManagement", "usb__core_8h.html#a20b4bdfac02495e248d9cc0c2f10bda8", null ],
      [ "controlManagement", "usb__core_8h.html#a958194b94ef21f282168a1c15d9527f7", null ],
      [ "functionalDescriptor", "usb__core_8h.html#a7ecaf88d51cad3091ba16c3efabd27ad", null ],
      [ "cifin", "usb__core_8h.html#a7fc244ee56a9337010aff080038b6400", null ],
      [ "dif", "usb__core_8h.html#ad9e994faf01eac6b9a5ae31d2c028e02", null ],
      [ "in", "usb__core_8h.html#af7c4a71ccaa04b9158318b02d64522e6", null ],
      [ "out", "usb__core_8h.html#a24ee64d6d086973e16e7a8cd232da817", null ]
    ] ],
    [ "USB_EP_SIZE", "usb__core_8h.html#aa659fe91c48da450cc471d7aba70b0f4", null ],
    [ "GET_STATUS", "usb__core_8h.html#aeba76c92af8f1a94982ec4cb767452f0", null ],
    [ "CLEAR_FEATURE", "usb__core_8h.html#a1908d37748e7d545ee5f190715624150", null ],
    [ "SET_FEATURE", "usb__core_8h.html#af8b97e67097fbf7d56c3dcac52fe679e", null ],
    [ "SET_ADDRESS", "usb__core_8h.html#ad3c9f5426c07d7d4da8cf1752a111576", null ],
    [ "GET_DESCRIPTOR", "usb__core_8h.html#a115a1d866ed9498300d59c90549ead0d", null ],
    [ "SET_DESCRIPTOR", "usb__core_8h.html#a264a9bda1309d7cc0311d57274194ca3", null ],
    [ "GET_CONFIGURATION", "usb__core_8h.html#ac6bffdcb940a0338051c4baa72498beb", null ],
    [ "SET_CONFIGURATION", "usb__core_8h.html#a5cdfe3de183eb4e190b2ccaa299045bf", null ],
    [ "GET_INTERFACE", "usb__core_8h.html#a1846f3a13493771ceced447397221de1", null ],
    [ "SET_INTERFACE", "usb__core_8h.html#a90c9c23759bdbb3ba106f88cb5ffc261", null ],
    [ "REQUEST_HOSTTODEVICE", "usb__core_8h.html#a726c73bc862ffdb626ae23f7c9d3fb1e", null ],
    [ "REQUEST_DEVICETOHOST", "usb__core_8h.html#a4c6ea3bc81f190df4b672b610a5be157", null ],
    [ "REQUEST_DIRECTION", "usb__core_8h.html#a843ac11b6071e30f9c60e2f194c4d3d9", null ],
    [ "REQUEST_STANDARD", "usb__core_8h.html#a1dd6ef57678e7af95d835049d5b355b9", null ],
    [ "REQUEST_CLASS", "usb__core_8h.html#afd24e03239e2df9e6d9c6cbcd53c1337", null ],
    [ "REQUEST_VENDOR", "usb__core_8h.html#a40131e4daec200a7d931c2b26e43d27f", null ],
    [ "REQUEST_TYPE", "usb__core_8h.html#a77263d461d2c46ff6fbd2a5b7080e2a8", null ],
    [ "REQUEST_DEVICE", "usb__core_8h.html#aa9d346dd935cf10fd2918cfb68254be7", null ],
    [ "REQUEST_INTERFACE", "usb__core_8h.html#acc603a2523c52f6d06d4cb25dabe26ed", null ],
    [ "REQUEST_ENDPOINT", "usb__core_8h.html#a45062eac8a589bbbd732dd1810bee872", null ],
    [ "REQUEST_OTHER", "usb__core_8h.html#ac83167ac664e91891ac561d29ed53e99", null ],
    [ "REQUEST_RECIPIENT", "usb__core_8h.html#aad5c36b4f5d06a0bc898bddb5f46e790", null ],
    [ "REQUEST_DEVICETOHOST_CLASS_INTERFACE", "usb__core_8h.html#abbe36a6ce3458c738f0c783527e26cdc", null ],
    [ "REQUEST_HOSTTODEVICE_CLASS_INTERFACE", "usb__core_8h.html#ab0e7948070353c754edb94260c759518", null ],
    [ "REQUEST_DEVICETOHOST_STANDARD_INTERFACE", "usb__core_8h.html#adb28397dccbdf31e1d579822d3723750", null ],
    [ "CDC_SET_LINE_CODING", "usb__core_8h.html#ad07ce089c8757b8d2ff37ddd83dc9351", null ],
    [ "CDC_GET_LINE_CODING", "usb__core_8h.html#a1068ab45668e413d964a01bc36fa2d43", null ],
    [ "CDC_SET_CONTROL_LINE_STATE", "usb__core_8h.html#a11c392243dec4d03970f1eaa1f8053ae", null ],
    [ "CDC_SEND_BREAK", "usb__core_8h.html#a868c480caf0abda6b458c82281dda8ad", null ],
    [ "MSC_RESET", "usb__core_8h.html#ad0cadf2666eeb4dd045fb6af12a775aa", null ],
    [ "MSC_GET_MAX_LUN", "usb__core_8h.html#aa6e8822acdf9b830202d1def16c3ca0e", null ],
    [ "USB_DEVICE_DESC_SIZE", "usb__core_8h.html#acd04660262562260957b6df8d5e482b3", null ],
    [ "USB_CONFIGUARTION_DESC_SIZE", "usb__core_8h.html#a6ce7e811f8523649e7eaa158dd06b8c6", null ],
    [ "USB_INTERFACE_DESC_SIZE", "usb__core_8h.html#ac7fd274cdd131f4c0addec7a139bc9b7", null ],
    [ "USB_ENDPOINT_DESC_SIZE", "usb__core_8h.html#a317f2697db907bff3f818a644d231c08", null ],
    [ "USB_DEVICE_DESCRIPTOR_TYPE", "usb__core_8h.html#a722f2d6ee2892228b6708e8b9904c645", null ],
    [ "USB_CONFIGURATION_DESCRIPTOR_TYPE", "usb__core_8h.html#a45e81401e4b530280fdd539da1b06c26", null ],
    [ "USB_STRING_DESCRIPTOR_TYPE", "usb__core_8h.html#ab01c9c74f2eb266b20aecd48bab4b35c", null ],
    [ "USB_INTERFACE_DESCRIPTOR_TYPE", "usb__core_8h.html#ad1699884fa580bba35246a566264c978", null ],
    [ "USB_ENDPOINT_DESCRIPTOR_TYPE", "usb__core_8h.html#a4e8d6d81a224f8b511edc92b6cb4e085", null ],
    [ "DEVICE_REMOTE_WAKEUP", "usb__core_8h.html#a77fa7e6093059898a0849f02968fba1d", null ],
    [ "ENDPOINT_HALT", "usb__core_8h.html#a4d7a6aea8f3ed0b277c3b40fb5df77fc", null ],
    [ "TEST_MODE", "usb__core_8h.html#ab6d58cce6e97b6b549801e696ac9f4f6", null ],
    [ "FEATURE_SELFPOWERED_ENABLED", "usb__core_8h.html#a3cb190e7ca03f8911929d96f6b8569bb", null ],
    [ "FEATURE_REMOTE_WAKEUP_ENABLED", "usb__core_8h.html#a61f70ea6d0155178833ac6b28dc4c435", null ],
    [ "USB_DEVICE_CLASS_COMMUNICATIONS", "usb__core_8h.html#ab6fd7e18adf5c748a6b56391c1c2b819", null ],
    [ "USB_DEVICE_CLASS_HUMAN_INTERFACE", "usb__core_8h.html#a3d9e0560a81b4dfd30bbf829960dbfab", null ],
    [ "USB_DEVICE_CLASS_STORAGE", "usb__core_8h.html#a053d2a9c3c9856ee01589ebb64d2e09b", null ],
    [ "USB_DEVICE_CLASS_VENDOR_SPECIFIC", "usb__core_8h.html#a89b94b8bc3c5fa2ef7151b5d282f22bb", null ],
    [ "USB_CONFIG_POWERED_MASK", "usb__core_8h.html#afdd4f846b5497985c182a1a090d99729", null ],
    [ "USB_CONFIG_BUS_POWERED", "usb__core_8h.html#a63c0a08ce70650c3c677ab523a45a0bb", null ],
    [ "USB_CONFIG_SELF_POWERED", "usb__core_8h.html#a8c322771b797c09e3066a388b46e9ef9", null ],
    [ "USB_CONFIG_REMOTE_WAKEUP", "usb__core_8h.html#a7b28a766a8a916fae421d52fbedd2cba", null ],
    [ "USB_CONFIG_POWER_MA", "usb__core_8h.html#adcf08dc7fdb6117608624203612b3104", null ],
    [ "USB_ENDPOINT_DIRECTION_MASK", "usb__core_8h.html#ab7544c5b8cf975839422c1ce29d4c5cb", null ],
    [ "USB_ENDPOINT_OUT", "usb__core_8h.html#a81ac2a8c9688d35940a054c4194c278d", null ],
    [ "USB_ENDPOINT_IN", "usb__core_8h.html#abe258bf57244c49874da6d53679e6df8", null ],
    [ "USB_ENDPOINT_TYPE_MASK", "usb__core_8h.html#acd114fdf068aaa69407358ee5ccfd170", null ],
    [ "USB_ENDPOINT_TYPE_CONTROL", "usb__core_8h.html#a6a51ed22b06e5ca0f76a35b6bac8d73a", null ],
    [ "USB_ENDPOINT_TYPE_ISOCHRONOUS", "usb__core_8h.html#ab05aae4a5d079b8dfb6a9cb1325ea6e5", null ],
    [ "USB_ENDPOINT_TYPE_BULK", "usb__core_8h.html#af4bac84d4576dc8f74f39dc75749e3dc", null ],
    [ "USB_ENDPOINT_TYPE_INTERRUPT", "usb__core_8h.html#a2cb5aa69a03df20aab217b808ad692a6", null ],
    [ "CDC_V1_10", "usb__core_8h.html#a8e40069024a8bbca6a4c9475f16a390b", null ],
    [ "CDC_COMMUNICATION_INTERFACE_CLASS", "usb__core_8h.html#a15ba271255934c53442a1b5c3f84bcdd", null ],
    [ "CDC_CALL_MANAGEMENT", "usb__core_8h.html#a9917cd2343a8736133247b9c3fe77d07", null ],
    [ "CDC_ABSTRACT_CONTROL_MODEL", "usb__core_8h.html#a45c103c50f65018dc180c9b2bebf7efd", null ],
    [ "CDC_HEADER", "usb__core_8h.html#aed9b4832a62cea26549cb48d1d60b3cd", null ],
    [ "CDC_ABSTRACT_CONTROL_MANAGEMENT", "usb__core_8h.html#af25239a0a435bd921892cadadb9e253e", null ],
    [ "CDC_UNION", "usb__core_8h.html#af6f38e6604e2bc2f4dc6e138cc39162a", null ],
    [ "CDC_CS_INTERFACE", "usb__core_8h.html#afe0ca40b5aacc5c750f952af952583d2", null ],
    [ "CDC_CS_ENDPOINT", "usb__core_8h.html#ae07e10978a48ff63df33c3aeedf84c3a", null ],
    [ "CDC_DATA_INTERFACE_CLASS", "usb__core_8h.html#af81d6e2f13ca36ac1099de2cabd56b84", null ],
    [ "MSC_SUBCLASS_SCSI", "usb__core_8h.html#a8612042c34acce715e8d818675611224", null ],
    [ "MSC_PROTOCOL_BULK_ONLY", "usb__core_8h.html#aec9cded93e5e4ce1bee3f5599e5cf12c", null ],
    [ "USB_VERSION", "usb__core_8h.html#a6bc3df68ee1deeb69cebfdfef7f8b685", null ],
    [ "TRANSFER_PGM", "usb__core_8h.html#a3a47b337666df84dc1367ff641aba099", null ],
    [ "TRANSFER_RELEASE", "usb__core_8h.html#ad41c80d70dd627b2b5776ff6d4711cca", null ],
    [ "TRANSFER_ZERO", "usb__core_8h.html#ae08385db4bf2c475f95601178b76b561", null ],
    [ "D_DEVICE", "usb__core_8h.html#abcfb18ee0e0424f00f7d218908588a26", null ],
    [ "D_CONFIG", "usb__core_8h.html#a8b4e445c5a7ccd0a9a577d1b1e5bb3ab", null ],
    [ "D_INTERFACE", "usb__core_8h.html#a00ea9102e9b3a0343e1f05d3a5af883f", null ],
    [ "D_ENDPOINT", "usb__core_8h.html#a402a2f4e31a74ce157c6a74ba5e90f54", null ],
    [ "D_IAD", "usb__core_8h.html#af9b99e1780a5465f84d24b123ea4c26a", null ],
    [ "D_CDCCS", "usb__core_8h.html#a47acd0440d3f16d1c34ef25484638658", null ],
    [ "D_CDCCS4", "usb__core_8h.html#af389cc2d8ee1f15ba6a2411e625032bc", null ]
];