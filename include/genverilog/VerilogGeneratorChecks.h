/**
 *  This Checks make some verifications for the Verilog Generator v1 to work
 *
 */

 #ifndef VERILOGGENERATORCHECKS_H_
#define VERILOGGENERATORCHECKS_H_

// Includes
//-----------------



#include "VerificatorRule.h"

/**
 * This rule verifies that for each State, none of its transitions + hypertransition are overlapping
 */
class VerilogGeneratorChecks: public VerificatorRule {
    public:
        VerilogGeneratorChecks();
        virtual ~VerilogGeneratorChecks();

        virtual QString getName();

        /**
         * Verifies all the overlapping transitions + hypertransitions
         * @param fsm
         * @return
         */
        virtual QList<RuleError*> applyRule(Fsm * fsm);



};

#endif /* VERILOGGENERATORCHECKS_H_ */
