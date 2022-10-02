/*
 * Vue Python C example
 * Copyright (C) 2018 by Eugene Yemelyanov <yemelgen@gmail.com>
 * This code is under MIT license (see LICENSE.txt for details)
 */

Vue.use(Vuetify);

let frame = Vue.component('home', {
	template: '#frame',
	data: () => ({
		version: '',
		code: '',
		db: '',
		expDate: new Date().toISOString().split('T')[0],
		noExpDate: false,
		output: '',
		workplaces: 200,
		selectedOptions: [],
		picker: false,
		valid: true,
		rules: {
			required: v => !!v || 'Field is required',
			number: v => /^[0-9]+$/.test(v) || 'Must be a number',
			hexnumber: v => /^[0-9A-Fa-f]+$/.test(v) || 'Must be a hexdecimal number',
			version: v => v.length == 8 || 'Exactly 8 characters',
			code: v => v.length == 4 || 'Exactly 4 characters',
			options: v => v.length > 0 || 'At least one option is required',
		}
	}),
	computed: {
		expDateISO: ({ expDate }) => new Date( expDate ).toISOString(),
		options: ({ $store }) => $store.state.menu.data.items || [],
		selectedAllOptions: function () {
			return this.selectedOptions.length === this.options.length;
		},
		selectedSomeOptions: function () {
			return this.selectedOptions.length > 0 && !this.selectedAllOptions;
		},
		selectIcon: function() {
			if ( this.selectedAllOptions ) return 'check_box';
			if ( this.selectedSomeOptions ) return 'indeterminate_check_box';
			return 'check_box_outline_blank';
		},
	},
	mounted: async function() {
		await this.$store.dispatch('menu/getList');
		this.reset();
	},
	methods: {
		create: async function() {
			await this.$store.dispatch('keys/create', {
				version: this.version,
				code: this.code,
				db: this.db,
				expdate: this.expDateISO,
				noexpdate: this.noExpDate,
				workplaces: this.workplaces,
				options: this.selectedOptions,
			});
			this.output = this.$store.state.keys.data.item;
			navigator.clipboard.writeText( this.output );
		},
		copy: function() {
			navigator.clipboard.writeText( this.output );
		},
		selectAll: function() {
			this.$nextTick(() => {
				if ( this.selectedAllOptions )
					this.selectedOptions = [];
				else
					this.selectedOptions = this.options.map( o => o.id).slice();
			});
		},
		reset: function() {
			this.version = '';
			this.code = '';
			this.db = '';
			this.expDate = new Date().toISOString().split('T')[0];
			this.noExpDate = false;
			this.workpalces = 200;
			this.selectedOptions = this.options.map( o => o.id);
			this.output = '';
			this.$refs.form.resetValidation();
			//this.$refs.form.reset();
		},
	},
	watch: {
		code: function(v) {
			this.code = v.toUpperCase();
		},
	}
});
