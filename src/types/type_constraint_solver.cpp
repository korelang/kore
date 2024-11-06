void TypeConstraintSolver::solve(const TypeConstraintSet& constraints) {
	// TODO: This should probably be a union find instead. Grab the union find from cvx
	unionfind<TypeVariable, Type> substitution;
	
	for (const auto& type_constraint : constraints) {
		unify(type_constraint);
	}
	
	return substitution;
}

void TypeConstraintSolver::unify(const TypeConstraint& constraint) {
	if (substitution.empty()) {
		return; 
	} else if (*constraint.type1 == *constraint.type2) {
		// Types are already unified
		return; 
	} else if (constraint.type1->is_type_variable()) {
		unify_variable(constraint.type1, constraint.type2);
	} else if (constraint.type2->is_type_variable()) {
		unify_variable(constraint.type2, constraint.type1);
	} 
}

void TypeConstraintSolver::unify_variable(const Type* type1, const Type* type2) {
	auto typevar = type1->as<TypeVariable>;
	
	if (typevar != nullptr) {
	auto sub = substitution.find(type1->as<TypeVariable>.name());
	
	if (sub != substitution.end()) {
		// There was a substitution for the type variable so unify that return the other type
		unify(*sub, type2);
	} else {
	    sub = substitution.find(type2->as<TypeVariable>.name());
	
	    if (sub != substitution.end()) {
		    unify(*sub, type1);
		}
	} else if (occurs_check(type1, type2)) {
	} else {
		substitution[typevar.name()] = type2;
	}
}

void TypeConstraintSolver::unify_function_type(const Type* type1, const Type* type2) {
}

bool TypeConstraintSolver::occurs_check(const Type* type1, const Type* type2) {
}