#pragma once
#include "dx9asm_register_mapping.h"

namespace dxapex {

  namespace dx9asm {

    class RegisterMap {
    public:
      inline const RegisterMapping* getRegisterMapping(const DX9Operand& operand) const {
        return getRegisterMapping(operand);
      }

      inline RegisterMapping* getRegisterMapping(const DX9Operand& operand) {
        return getRegisterMapping(operand.getRegType(), operand.getRegNumber());
      }

      inline const RegisterMapping* getRegisterMapping(uint32_t type, uint32_t index) const {
        return getRegisterMapping(type, index);
      }

      inline RegisterMapping* getRegisterMapping(uint32_t type, uint32_t index) {
        for (RegisterMapping& mapping : m_registerMap) {
          if (mapping.dx9Type == type && mapping.dx9Id == index)
            return &mapping;
        }
        return nullptr;
      }

      inline uint32_t getHighestIdForDXBCType(uint32_t type) const {
        const uint32_t invalidId = UINT32_MAX;

        uint32_t highestIdForType = invalidId;

        for (const RegisterMapping& lum : m_registerMap) {
          const DXBCOperand& lumOperand = lum.dxbcOperand;

          if (lumOperand.getRegisterType() == type) {
            highestIdForType = max(highestIdForType, lumOperand.getRegNumber());

            if (highestIdForType == invalidId)
              highestIdForType = lumOperand.getRegNumber();
          }
        }

        return highestIdForType;
      }

      inline uint32_t getDXBCTypeCount(uint32_t type) const {
        return getHighestIdForDXBCType(type) + 1;
      }


      RegisterMapping* lookupOrCreateRegisterMapping(const DX9Operand& operand, uint32_t regOffset = 0);

      inline void addRegisterMapping(bool generateDXBCId, RegisterMapping& mapping) {
        DXBCOperand& dxbcOperand = mapping.dxbcOperand;
        if (generateDXBCId) {
          uint32_t& regNumber = dxbcOperand.getRegNumber();
          uint32_t highestIdForType = getHighestIdForDXBCType(dxbcOperand.getRegisterType());

          highestIdForType++;
          regNumber = highestIdForType;
        }

        m_registerMap.push_back(mapping);
      }

      inline std::vector<RegisterMapping>& getRegisterMappings() {
        return m_registerMap;
      }
    private:
      std::vector<RegisterMapping> m_registerMap;
    };

  }

}